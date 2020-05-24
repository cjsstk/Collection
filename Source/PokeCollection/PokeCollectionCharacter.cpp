// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "PokeCollectionCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "PokeCharacter.h"
#include "PokeCore.h"
#include "PokeCollectionGameMode.h"
#include "PokeEquipment.h"
#include "PokeItem.h"
#include "PokePlayerController.h"
#include "CMS.h"

static TAutoConsoleVariable<int32> CVarMaxBattleSpeedMultiplier
(
	TEXT("poke.maxBattleSpeedMultiplier"),
	3,
	TEXT("MaxBattleSpeedMultiplier")
);

//////////////////////////////////////////////////////////////////////////
// APokeCollectionCharacter

APokeCollectionCharacter::APokeCollectionCharacter()
{
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(RootComponent);


}

void APokeCollectionCharacter::InitHaveCharacters()
{
	for (characterKey Key : SavedCharacterKeys)
	{
		FInitCharacterParams Params;
		Params.CharacterKey = Key;

		AddNewCharacter(Params);
	}
}

void APokeCollectionCharacter::InitHaveEquipments()
{
	for (equipmentKey Key : SavedEquipmentKeys)
	{
		FInitEquipmentParams Params;
		Params.EquipmentKey = Key;

		AddNewEquipment(Params);
	}
}

void APokeCollectionCharacter::InitHaveItems()
{
	for (itemKey Key : SavedItemKeys)
	{
		FInitItemParams Params;
		Params.ItemKey = Key;

		AddNewItem(Params);
	}
}

void APokeCollectionCharacter::InitPlayerInfo()
{
	SetPlayerLevel(1);
}

void APokeCollectionCharacter::InitMainCharacter()
{
	if (SavedCharacterKeys[0])
		MainCharacterID = SavedCharacterKeys[0];
}

void APokeCollectionCharacter::SetPlayerMaxExp(int32 InMaxExp)
{
	PlayerMaxExp = InMaxExp;
}

void APokeCollectionCharacter::SetPlayerMode(EPlayerMode NewPlayerMode)
{
	APokePlayerController* PC = Cast<APokePlayerController>(GetController());
	if (!PC)
	{
		return;
	}

	switch (NewPlayerMode)
	{
	case EPlayerMode::BattleMode:
		CameraComponent->SetRelativeRotation(FRotator(0, 0, 0));
		PC->SetInputMode(FInputModeGameAndUI());
		break;
	case EPlayerMode::MakePartyMode:
		CameraComponent->SetRelativeRotation(FRotator(0, 180, 0));
		PC->SetInputMode(FInputModeGameOnly());
		break;
	case EPlayerMode::UIMode:
		CameraComponent->SetRelativeRotation(FRotator(0, 90, 0));
		PC->SetInputMode(FInputModeUIOnly());

		break;
	default:
		break;
	}
}

void APokeCollectionCharacter::AddNewCharacter(FInitCharacterParams& InInitCharacterParams)
{
	const FCharacterInfo* CharacterInfo = CMS::GetCharacterDataTable(InInitCharacterParams.CharacterKey);
	if (!CharacterInfo)
	{
		ensure(0);
		return;
	}

	if (InInitCharacterParams.CharacterID < 0)
	{
		InInitCharacterParams.CharacterID = GetUsableCharacterID();
	}

	APokeCharacter* PokeCharacter = NewObject<APokeCharacter>();
	if (PokeCharacter)
	{
		PokeCharacter->Init(InInitCharacterParams);
		//PokeCharacter->SetCharacterID(InInitCharacterParams.CharacterID);
		//++NextCharacterID;
	}

	HaveCharacters.AddUnique(PokeCharacter);
	AddCharacterToIndex(InInitCharacterParams.CharacterKey);

	OnAddedNewCharacter.Broadcast();
}

void APokeCollectionCharacter::AddNewEquipment(FInitEquipmentParams& InInitEquipmentParams)
{
	const FEquipmentInfo* EquipmentInfo = CMS::GetEquipmentDataTable(InInitEquipmentParams.EquipmentKey);
	if (!ensure(EquipmentInfo))
	{
		return;
	}

	if (InInitEquipmentParams.EquipmentID < 0)
	{
		InInitEquipmentParams.EquipmentID = GetUsableEquipmentID();
	}

	UPokeEquipment* PokeEquipment = NewObject<UPokeEquipment>();
	if (PokeEquipment)
	{
		PokeEquipment->Init(InInitEquipmentParams);
	}

	HaveEquipments.AddUnique(PokeEquipment);
}

void APokeCollectionCharacter::AddNewItem(FInitItemParams& InInitItemParams)
{
	const FPokeItemInfo* ItemInfo = CMS::GetItemDataTable(InInitItemParams.ItemKey);
	if (!ensure(ItemInfo))
	{
		return;
	}

	UPokeItem* HaveItem = GetItemByKey(InInitItemParams.ItemKey);
	if (HaveItem)
	{
		HaveItem->SetStackNum(HaveItem->GetStackNum() + InInitItemParams.ItemStackNum);
	}
	else
	{
		if (InInitItemParams.ItemID < 0)
		{
			InInitItemParams.ItemID = GetUsableItemID();
		}

		UPokeItem* PokeItem = NewObject<UPokeItem>();
		if (PokeItem)
		{
			PokeItem->Init(InInitItemParams);
		}

		HaveItems.AddUnique(PokeItem);
	}
}

void APokeCollectionCharacter::GetReward(FBattleReward InBattleReward)
{
	const TMap<int32, APokeCharacter*>& CurrentPartyCharacters = GetPartyCharacters(CurrentSelectedParty);

	for (auto&& PartyMember : CurrentPartyCharacters)
	{
		APokeCharacter* PartyCharacter = PartyMember.Value;
		if (!PartyCharacter)
		{
			continue;
		}

		PartyCharacter->TakeExperience(InBattleReward.ExperienceAmount);
	}

	for (int32 NewCharacterKey : InBattleReward.GetCharacters)
	{
		FInitCharacterParams Params;
		Params.CharacterKey = NewCharacterKey;

		AddNewCharacter(Params);
	}
}

void APokeCollectionCharacter::SetMaxClearBattleStage(battleStageKey InBattleStageKey)
{
	MaxClearBattleStageNum = FMath::Max(MaxClearBattleStageNum, InBattleStageKey);
}

void APokeCollectionCharacter::PutOnEquipment(int32 InCharacterID, int32 InEquipmentID)
{
	APokeCharacter* PokeCharacter = GetCharacterByID(InCharacterID);
	if (!PokeCharacter)
	{
		return;
	}

	UPokeEquipment* PokeEquipment = GetEquipmentByID(InEquipmentID);
	if (!PokeEquipment)
	{
		return;
	}

	if (PokeEquipment->GetOwnerCharacterID() >= 0)
	{
		APokeCharacter* PrevCharacter = GetCharacterByID(PokeEquipment->GetOwnerCharacterID());
		if (!ensure(PrevCharacter))
		{
			return;
		}

		PrevCharacter->TakeOffEquipment();
	}

	PokeCharacter->PutOnEquipment(PokeEquipment);
}

void APokeCollectionCharacter::TakeOffEquipment(int32 InCharacterID)
{
	APokeCharacter* PokeCharacter = GetCharacterByID(InCharacterID);
	if (!PokeCharacter)
	{
		return;
	}

	PokeCharacter->TakeOffEquipment();
}

void APokeCollectionCharacter::SetPlayerLevel(int32 NewLevel)
{
	PlayerLevel = NewLevel;

	const FPlayerExperienceTable* ExpTable = CMS::GetPlayerExperienceTable(NewLevel);
	if (ExpTable)
	{
		SetPlayerMaxExp(ExpTable->NeedExperienceForNextLevel);
	}
	else
	{
		SetPlayerMaxExp(99999);
	}

}

void APokeCollectionCharacter::SetMainCharacterID(int32 NewMainCharacterID)
{
	APokeCharacter* PokeCharacter = GetCharacterByID(NewMainCharacterID);
	if (!PokeCharacter)
	{
		return;
	}

	MainCharacterID = NewMainCharacterID;
}

void APokeCollectionCharacter::SetCurrentSelectedBattleStageKey(battleStageKey InBattleStageKey)
{
	if (InBattleStageKey == INVALID_BATTLESTAGEKEY)
	{
		return;
	}

	CurrentSelectedBattleStageKey = InBattleStageKey;
}

bool APokeCollectionCharacter::IsCompleteIndexCharacter(characterKey InCharacterKey)
{
	return (CharacterIndex.Contains(InCharacterKey) && *CharacterIndex.Find(InCharacterKey)); 
}

bool APokeCollectionCharacter::IsClearBattleStage(int32 InBattleStageKey)
{
	return (MaxClearBattleStageNum >= InBattleStageKey);
}

void APokeCollectionCharacter::ChangeBattleSpeedMultiplier()
{
	/** BattleSpeedMultiplier is 1 ~ 3 */
	BattleSpeedMultiplier = (BattleSpeedMultiplier % CVarMaxBattleSpeedMultiplier.GetValueOnGameThread()) + 1;
}

const TArray<class APokeCharacter*>& APokeCollectionCharacter::GetHaveCharacters() const
{
	return HaveCharacters; 
}

const TMap<int32, class APokeCharacter*> APokeCollectionCharacter::GetPartyCharacters(int32 InPartyNum) const
{
	TMap<int32, class APokeCharacter*> FoundPartyCharacters;

	for (auto&& Character : HaveCharacters)
	{
		if (Character->GetJoinedPartyNum() == InPartyNum)
		{
			FoundPartyCharacters.Add(Character->GetJoinedSlotNum(), Character);
		}
	}

	return FoundPartyCharacters;
}

APokeCharacter* APokeCollectionCharacter::GetCharacterByID(int32 InCharacterID) const
{
	for (APokeCharacter* Character : HaveCharacters)
	{
		if (Character && Character->GetCharacterID() == InCharacterID)
		{
			return Character;
		}
	}

	return nullptr;
}

APokeCharacter* APokeCollectionCharacter::GetCharacterBySlotNum(int32 InPartyNum, int32 InSlotNum) const
{
	APokeCharacter* const* FoundCharacter = HaveCharacters.FindByPredicate([InPartyNum, InSlotNum](APokeCharacter* PC)
		{
			return (PC->GetJoinedPartyNum() == InPartyNum && PC->GetJoinedSlotNum() == InSlotNum);
		}
	);

	if (!FoundCharacter)
	{
		return nullptr;
	}

	return *FoundCharacter;
}

const TArray<class UPokeEquipment*>& APokeCollectionCharacter::GetHaveEquipments() const
{
	return HaveEquipments;
}

class UPokeEquipment* APokeCollectionCharacter::GetEquipmentByID(int32 InEquipmentID) const
{
	for (UPokeEquipment* Equipment : HaveEquipments)
	{
		if (Equipment && Equipment->GetEquipmentID() == InEquipmentID)
		{
			return Equipment;
		}
	}

	return nullptr;
}

const TArray<class UPokeItem*>& APokeCollectionCharacter::GetHaveItems() const
{
	return HaveItems;
}

UPokeItem* APokeCollectionCharacter::GetItemByKey(int32 InItemKey)
{
	for (UPokeItem* Item : HaveItems)
	{
		if (Item && Item->GetItemKey() == InItemKey)
		{
			return Item;
		}
	}

	return nullptr;
}

UPokeItem* APokeCollectionCharacter::GetItemByID(int32 InItemID)
{
	for (UPokeItem* Item : HaveItems)
	{
		if (Item && Item->GetItemID() == InItemID)
		{
			return Item;
		}
	}

	return nullptr;
}

void APokeCollectionCharacter::ConsumeBerry(int32 InConsumeBerryAmount)
{
	if (BerryAmount < InConsumeBerryAmount)
	{
		ensure(0);
	}

	SetBerryAmount(BerryAmount - InConsumeBerryAmount);
}

void APokeCollectionCharacter::BeginPlay()
{
	Super::BeginPlay();

	FString MapName = GetWorld()->GetMapName();
	if (!MapName.Contains(FString(TEXT("InGameLevel"))))
	{
		return;
	}

	TArray<FCharacterInfo*> AllCharacterData;
	CMS::GetAllCharacterDataTable(AllCharacterData);
	for (FCharacterInfo* CharacterData : AllCharacterData)
	{
		if (!CharacterData)
		{
			continue;
		}

		CharacterIndex.Emplace(CharacterData->CharacterKey, false);
	}

	AHttpActor* HttpActor = PokeCore::GetHttpActor(GetWorld());
	if (!ensure(HttpActor))
	{
		return;
	}

	HttpActor->OnHttpLoginResponseReceived.BindUObject(this, &APokeCollectionCharacter::OnLoginResponsed);
	HttpActor->OnHttpHaveCharactersResponseReceived.BindUObject(this, &APokeCollectionCharacter::OnHaveCharactersResponsed);
	HttpActor->OnHttpHaveEquipmentsResponseReceived.BindUObject(this, &APokeCollectionCharacter::OnHaveEquipmentsResponsed);

	HttpActor->RequestLogin(PokeCore::DeviceId);
	//HttpActor->RequestHaveCharacters(PokeCore::DeviceId);

	InitHaveCharacters();
	InitMainCharacter();
	InitHaveEquipments();
	InitHaveItems();
	InitPlayerInfo();

	// Temp slot setting
	for (int32 i = 1; i < 5; i++)
	{
		HaveCharacters[i * 2]->SetJoinedPartyNum(1);
		HaveCharacters[i * 2]->SetJoinedSlotNum(i);
	}

	//PutOnEquipment(1, 1);
}

void APokeCollectionCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	TickResourceCharge(DeltaSeconds);
}

void APokeCollectionCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);


}

void APokeCollectionCharacter::TickResourceCharge(float DeltaSeconds)
{
	BerryChargingIntervalAgeSeconds += DeltaSeconds;
	MoneyChargingIntervalAgeSeconds += DeltaSeconds;

	while (BerryChargingIntervalAgeSeconds >= BerryChargingIntervalSeconds)
	{
		BerryChargingIntervalAgeSeconds -= BerryChargingIntervalSeconds;
		BerryAmount += BerryChargingAmount;
	}

	while (MoneyChargingIntervalAgeSeconds >= MoneyChargingIntervalSeconds)
	{
		MoneyChargingIntervalAgeSeconds -= MoneyChargingIntervalSeconds;
		MoneyAmount += MoneyChargingAmount;
	}
}

void APokeCollectionCharacter::AddCharacterToIndex(characterKey InCharacterKey)
{
	bool* IndexCharacter = CharacterIndex.Find(InCharacterKey);
	if (ensure(IndexCharacter))
	{
		*IndexCharacter = true;
	}
}

int32 APokeCollectionCharacter::GetUsableCharacterID()
{
	int32 UsableCharacterID = 0;

	while (HaveCharacters.FindByPredicate([UsableCharacterID](APokeCharacter* PC) { return (PC->GetCharacterID() == UsableCharacterID); }))
	{
		UsableCharacterID++;
	}
	
	return UsableCharacterID;
}

int32 APokeCollectionCharacter::GetUsableEquipmentID()
{
	int32 UsableEquipmentID = 0;

	while (HaveEquipments.FindByPredicate([UsableEquipmentID](UPokeEquipment* PE) { return (PE->GetEquipmentID() == UsableEquipmentID); }))
	{
		UsableEquipmentID++;
	}

	return UsableEquipmentID;
}

int32 APokeCollectionCharacter::GetUsableItemID()
{
	int32 UsableItemID = 0;

	while (HaveItems.FindByPredicate([UsableItemID](UPokeItem* PokeItem) { return (PokeItem->GetItemID() == UsableItemID); }))
	{
		UsableItemID++;
	}

	return UsableItemID;
}

void APokeCollectionCharacter::OnLoginResponsed(FHttpRequestPtr Request, TSharedPtr<FJsonObject> ResponceJson, bool bWasSuccessful)
{
	if (!ResponceJson)
	{
		return;
	}

	int32 recievedCode = ResponceJson->GetIntegerField("code");
	if (recievedCode == 200)
	{
		TSharedPtr<FJsonObject> b = ResponceJson->GetObjectField("data");

		int32 c = b->GetIntegerField("level");
	}
	else
	{
		ensure(0);
	}
}

void APokeCollectionCharacter::OnHaveCharactersResponsed(FHttpRequestPtr Request, TSharedPtr<FJsonObject> ResponceJson, bool bWasSuccessful)
{
	/*APokeCharacter* PokeCharacter = NewObject<APokeCharacter>();
	if (PokeCharacter)
	{
		FInitCharacterParams Params;

		PokeCharacter->Init(Params);
		HaveCharacters.AddUnique(PokeCharacter);
	}*/
}

void APokeCollectionCharacter::OnHaveEquipmentsResponsed(FHttpRequestPtr Request, TSharedPtr<FJsonObject> ResponceJson, bool bWasSuccessful)
{

}
