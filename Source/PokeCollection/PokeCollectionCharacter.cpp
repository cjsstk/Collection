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
#include "PokeCollectionGameMode.h"
#include "PokeEquipment.h"
#include "PokePlayerController.h"
#include "CMS.h"

//////////////////////////////////////////////////////////////////////////
// APokeCollectionCharacter

APokeCollectionCharacter::APokeCollectionCharacter()
{
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(RootComponent);


}

void APokeCollectionCharacter::InitHaveCharacters()
{
	NextCharacterID = 0;

	for (characterKey Key : SavedCharacterKeys)
	{
		FInitCharacterParams Params;
		Params.CharacterKey = Key;

		AddNewCharacter(Params);
	}
}

void APokeCollectionCharacter::InitHaveEquipments()
{
	NextEquipmentID = 0;

	for (equipmentKey Key : SavedEquipmentKeys)
	{
		const FEquipmentInfo* EquipmentInfo = CMS::GetEquipmentDataTable(Key);
		if (!EquipmentInfo)
		{
			ensure(0);
			continue;
		}

		UPokeEquipment* PokeEquipment = NewObject<UPokeEquipment>();
		if (PokeEquipment)
		{
			PokeEquipment->Init(Key);
			PokeEquipment->SetEquipmentID(NextEquipmentID);
			++NextEquipmentID;
		}

		HaveEquipments.AddUnique(PokeEquipment);
	}
}

void APokeCollectionCharacter::InitPlayerInfo()
{
	SetPlayerLevel(1);
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

	APokeCharacter* PokeCharacter = NewObject<APokeCharacter>();
	if (PokeCharacter)
	{
		PokeCharacter->Init(InInitCharacterParams);
		PokeCharacter->SetCharacterID(NextCharacterID);
		++NextCharacterID;
	}

	HaveCharacters.AddUnique(PokeCharacter);
	AddCharacterToIndex(InInitCharacterParams.CharacterKey);

	OnAddedNewCharacter.Broadcast();
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


	InitHaveCharacters();
	InitHaveEquipments();
	InitPlayerInfo();

	// Temp slot setting
	for (int32 i = 1; i < 5; i++)
	{
		HaveCharacters[i * 2]->SetJoinedPartyNum(1);
		HaveCharacters[i * 2]->SetJoinedSlotNum(i);
	}

	PutOnEquipment(1, 1);
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
