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
#include "PlayerQuestComponent.h"
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

	QuestComponent = CreateDefaultSubobject<UPlayerQuestComponent>(TEXT("QuestComponent"));
}

void APokeCollectionCharacter::InitHaveCharacters()
{
	AHttpActor* HttpActor = PokeCore::GetHttpActor(GetWorld());
	if (!ensure(HttpActor))
	{
		return;
	}

	FHttpRequestParams Params;
	Params.RequestType = EHttpRequestType::HaveCharacters;
	Params.MemberID = PokeCore::DeviceId;

	HttpActor->Request(Params);
}

void APokeCollectionCharacter::InitHaveEquipments()
{
	AHttpActor* HttpActor = PokeCore::GetHttpActor(GetWorld());
	if (!ensure(HttpActor))
	{
		return;
	}

	FHttpRequestParams Params;
	Params.RequestType = EHttpRequestType::HaveEquipments;
	Params.MemberID = PokeCore::DeviceId;

	HttpActor->Request(Params);
}

void APokeCollectionCharacter::InitHaveItems()
{
	AHttpActor* HttpActor = PokeCore::GetHttpActor(GetWorld());
	if (!ensure(HttpActor))
	{
		return;
	}

	FHttpRequestParams Params;
	Params.RequestType = EHttpRequestType::HaveItems;
	Params.MemberID = PokeCore::DeviceId;

	HttpActor->Request(Params);

	/*for (auto&& Item : SavedItemKeys)
	{
		FInitItemParams Params;
		Params.ItemKey = Item.Key;
		Params.ItemStackNum = Item.Value;

		AddNewItem(Params);
	}*/
}

void APokeCollectionCharacter::InitHaveQuests()
{
	AHttpActor* HttpActor = PokeCore::GetHttpActor(GetWorld());
	if (!ensure(HttpActor))
	{
		return;
	}

	FHttpRequestParams Params;
	Params.RequestType = EHttpRequestType::HaveQuests;
	Params.MemberID = PokeCore::DeviceId;

	HttpActor->Request(Params);
}

void APokeCollectionCharacter::InitPlayerInfo(FInitPlayerParams& Params)
{
	PlayerNickName = FName(*Params.PlayerNickname);
	PlayerLevel = Params.PlayerLevel;

	const FPlayerExperienceTable* ExpTable = CMS::GetPlayerExperienceTable(PlayerLevel);
	if (ExpTable)
	{
		SetPlayerMaxExp(ExpTable->NeedExperienceForNextLevel);
	}
	else
	{
		SetPlayerMaxExp(99999);
	}

	PlayerCurrentExp = Params.PlayerCurrentExp;
	MoneyAmount = Params.PlayerMoney;
	BerryAmount = Params.PlayerBerry;
	StardustAmount = Params.PlayerStardust;
	MaxClearBattleStageNum = Params.MaxClearBattleStage;
	CurrentSelectedChapterNum = Params.LastSelectedChapter;
	MaxOpenedChapterNum = Params.MaxOpenedChapter;
	CurrentSelectedParty = Params.LastSelectedParty;
	BattleClearCount = Params.BattleClearCount;
	GetCharacterCount = Params.GetCharacterCount;
	MaxHaveCharactersNum = Params.MaxHaveCharactersNum;
	MaxHaveEquipmentsNum = Params.MaxHaveEquipmentsNum;
	SetIndex(Params.Index);
	BattleSpeedMultiplier = Params.BattleSpeed;

	bPlayerLoaded = true;
}

void APokeCollectionCharacter::InitMainCharacter()
{
	if (SavedCharacterKeys[0])
		MainCharacterID = SavedCharacterKeys[0];
}

void APokeCollectionCharacter::SetPlayerExp(int32 InExp)
{
	if (PlayerMaxExp < 0)
	{
		return;
	}

	PlayerCurrentExp = InExp;
	
	while (PlayerCurrentExp >= PlayerMaxExp)
	{
		PlayerCurrentExp -= PlayerMaxExp;

		SetPlayerLevel(PlayerLevel + 1);
	}

	SavePlayerInfo(ESavePlayerInfo::Exp);
}

void APokeCollectionCharacter::SetPlayerMaxExp(int32 InMaxExp)
{
	PlayerMaxExp = InMaxExp;
}

void APokeCollectionCharacter::SetBerryAmount(int32 NewBerryAmount)
{
	BerryAmount = FMath::Clamp(NewBerryAmount, 0, INT32_MAX);

	SavePlayerInfo(ESavePlayerInfo::Berry);
}

void APokeCollectionCharacter::SetStardustAmount(int32 NewStardustAmount)
{
	StardustAmount = FMath::Clamp(NewStardustAmount, 0, INT32_MAX);

	SavePlayerInfo(ESavePlayerInfo::Stardust);
}

void APokeCollectionCharacter::SetBattleClearCount(int32 NewBattleClearCount)
{
	BattleClearCount = NewBattleClearCount;

	SavePlayerInfo(ESavePlayerInfo::BattleClearCount);
}

void APokeCollectionCharacter::SetGetCharacterCount(int32 NewGetCharacterCount)
{
	GetCharacterCount = NewGetCharacterCount;

	SavePlayerInfo(ESavePlayerInfo::GetCharacterCount);
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

bool APokeCollectionCharacter::AddNewCharacter(FInitCharacterParams& InInitCharacterParams)
{
	const FCharacterInfo* CharacterInfo = CMS::GetCharacterDataTable(InInitCharacterParams.CharacterKey);
	if (!CharacterInfo)
	{
		ensure(0);
		return false;
	}

	if (InInitCharacterParams.CharacterID < 0)
	{
		InInitCharacterParams.CharacterID = GetUsableCharacterID();
	}

	APokeCharacter* PokeCharacter = NewObject<APokeCharacter>();
	if (!PokeCharacter)
	{
		return false;
	}

	PokeCharacter->Init(InInitCharacterParams);
	HaveCharacters.AddUnique(PokeCharacter);
	AddCharacterToIndex(InInitCharacterParams.CharacterKey);

	OnAddedNewCharacter.Broadcast();

	return true;
}

void APokeCollectionCharacter::AddNewCharacters(TArray<FInitCharacterParams>& InInitCharacterParams)
{
	if (InInitCharacterParams.Num() <= 0)
	{
		return;
	}

	AHttpActor* HttpActor = PokeCore::GetHttpActor(GetWorld());
	if (!ensure(HttpActor))
	{
		return;
	}

	TArray<FInitCharacterParams> AddedCharacters;

	for (auto&& InitParams : InInitCharacterParams)
	{
		const FCharacterInfo* CharacterInfo = CMS::GetCharacterDataTable(InitParams.CharacterKey);
		if (!CharacterInfo)
		{
			continue;
		}

		if (AddNewCharacter(InitParams))
		{
			AddedCharacters.Add(InitParams);
		}
	}

	int32 NewCharacterNum = AddedCharacters.Num();
	UPlayerQuestComponent* QuestComp = GetQuestComponent();
	if (QuestComp)
	{
		FUpdateQuestParams Params;
		Params.ObjectionType = EQuestObjectionType::GetCharacterCount;
		Params.InCount = NewCharacterNum;

		QuestComp->UpdateQuest(Params);
	}

	if (AddedCharacters.Num() > 0)
	{
		FHttpRequestParams RequestParams;
		RequestParams.RequestType = EHttpRequestType::AddNewCharacters;
		RequestParams.MemberID = PokeCore::DeviceId;
		RequestParams.NewCharactersInfos = AddedCharacters;

		HttpActor->Request(RequestParams);

		SavePlayerInfo(ESavePlayerInfo::Index);
	}
}

void APokeCollectionCharacter::AddNewEquipments(TArray<FInitEquipmentParams>& InInitEquipmentParams)
{
	if (InInitEquipmentParams.Num() <= 0)
	{
		return;
	}

	AHttpActor* HttpActor = PokeCore::GetHttpActor(GetWorld());
	if (!ensure(HttpActor))
	{
		return;
	}

	TArray<FInitEquipmentParams> AddedEquipments;

	for (auto&& InitParams : InInitEquipmentParams)
	{
		const FEquipmentInfo* EquipmentInfo = CMS::GetEquipmentDataTable(InitParams.EquipmentKey);
		if (!EquipmentInfo)
		{
			continue;;
		}

		if (AddNewEquipment(InitParams))
		{
			AddedEquipments.Add(InitParams);
		}
	}

	if (AddedEquipments.Num() > 0)
	{
		FHttpRequestParams RequestParams;
		RequestParams.RequestType = EHttpRequestType::AddNewEquipments;
		RequestParams.MemberID = PokeCore::DeviceId;
		RequestParams.NewEquipmentsInfos = AddedEquipments;

		HttpActor->Request(RequestParams);
	}
}

void APokeCollectionCharacter::AddNewItems(TArray<FInitItemParams>& InInitItemParams)
{
	if (InInitItemParams.Num() <= 0)
	{
		return;
	}

	AHttpActor* HttpActor = PokeCore::GetHttpActor(GetWorld());
	if (!ensure(HttpActor))
	{
		return;
	}

	TArray<FInitItemParams> AddedItems;

	for (auto&& InitParams : InInitItemParams)
	{
		const FPokeItemInfo* ItemInfo = CMS::GetItemDataTable(InitParams.ItemKey);
		if (!ItemInfo)
		{
			continue;;
		}

		if (AddNewItem(InitParams))
		{
			AddedItems.Add(InitParams);
		}
	}

	if (AddedItems.Num() > 0)
	{
		FHttpRequestParams RequestParams;
		RequestParams.RequestType = EHttpRequestType::AddNewItems;
		RequestParams.MemberID = PokeCore::DeviceId;
		RequestParams.NewItemsInfos = AddedItems;

		HttpActor->Request(RequestParams);
	}
}

bool APokeCollectionCharacter::AddNewEquipment(FInitEquipmentParams& InInitEquipmentParams)
{
	const FEquipmentInfo* EquipmentInfo = CMS::GetEquipmentDataTable(InInitEquipmentParams.EquipmentKey);
	if (!ensure(EquipmentInfo))
	{
		return false;
	}

	if (InInitEquipmentParams.EquipmentID < 0)
	{
		InInitEquipmentParams.EquipmentID = GetUsableEquipmentID();
	}

	UPokeEquipment* PokeEquipment = NewObject<UPokeEquipment>();
	if (!PokeEquipment)
	{
		return false;
	}

	PokeEquipment->Init(InInitEquipmentParams);
	HaveEquipments.AddUnique(PokeEquipment);

	return true;
}

bool APokeCollectionCharacter::AddNewItem(FInitItemParams& InInitItemParams)
{
	const FPokeItemInfo* ItemInfo = CMS::GetItemDataTable(InInitItemParams.ItemKey);
	if (!ensure(ItemInfo))
	{
		return false;
	}

	UPokeItem* HaveItem = GetItemByKey(InInitItemParams.ItemKey);
	if (HaveItem)
	{
		HaveItem->SetStackNum(HaveItem->GetStackNum() + InInitItemParams.ItemStackNum);
		InInitItemParams.ItemID = HaveItem->GetItemID();
		return true;
	}
	else
	{
		if (InInitItemParams.ItemID < 0)
		{
			InInitItemParams.ItemID = GetUsableItemID();
		}

		UPokeItem* PokeItem = NewObject<UPokeItem>();
		if (!PokeItem)
		{
			return false;
		}

		PokeItem->Init(InInitItemParams);
		HaveItems.AddUnique(PokeItem);
		
		return true;
	}

	return false;
}

void APokeCollectionCharacter::DeleteCharacters(TArray<int32>& InCharacterIDs)
{
	AHttpActor* HttpActor = PokeCore::GetHttpActor(GetWorld());
	if (!ensure(HttpActor))
	{
		return;
	}

	TArray<int32> DestroyCharacterIds;
	TArray<int32> PutOnEquipments;

	for (int32 CharacterID : InCharacterIDs)
	{
		APokeCharacter* PokeCharacter = *HaveCharacters.FindByPredicate([CharacterID](APokeCharacter* PC) { return PC->GetCharacterID() == CharacterID; });

		if (!PokeCharacter)
		{
			return;
		}
		
		UPokeEquipment* PokeEquipment = PokeCharacter->GetCurrentEquipment();
		if (PokeEquipment)
		{
			PokeCharacter->TakeOffEquipment();
			PutOnEquipments.Add(PokeEquipment->GetEquipmentID());
		}

		DestroyCharacterIds.Add(CharacterID);
		HaveCharacters.Remove(PokeCharacter);
	}

	InCharacterIDs.Empty();

	if (DestroyCharacterIds.Num() > 0)
	{
		FHttpRequestParams RequestParams;
		RequestParams.RequestType = EHttpRequestType::DestroyCharacters;
		RequestParams.MemberID = PokeCore::DeviceId;
		RequestParams.DestoryCharacterIds = DestroyCharacterIds;

		HttpActor->Request(RequestParams);
	}

	if (PutOnEquipments.Num() > 0)
	{
		FHttpRequestParams RequestParams;
		RequestParams.RequestType = EHttpRequestType::DestroyEquipments;
		RequestParams.MemberID = PokeCore::DeviceId;
		RequestParams.DestoryCharacterIds = PutOnEquipments;

		HttpActor->Request(RequestParams);

		DeleteEquipments(PutOnEquipments);
	}
}

void APokeCollectionCharacter::DeleteEquipments(TArray<int32>& InEquipmentIDs)
{
	AHttpActor* HttpActor = PokeCore::GetHttpActor(GetWorld());
	if (!ensure(HttpActor))
	{
		return;
	}

	TArray<int32> DestroyEquipmentIds;

	for (int32 EquipmentID : InEquipmentIDs)
	{
		UPokeEquipment* PokeEquipment = *HaveEquipments.FindByPredicate([EquipmentID](UPokeEquipment* PE) { return PE->GetEquipmentID() == EquipmentID; });

		if (!PokeEquipment)
		{
			return;
		}

		PokeEquipment->SetOwnerCharacterID(-1);
		DestroyEquipmentIds.Add(EquipmentID);
		HaveEquipments.Remove(PokeEquipment);
	}

	InEquipmentIDs.Empty();

	if (DestroyEquipmentIds.Num() > 0)
	{
		FHttpRequestParams RequestParams;
		RequestParams.RequestType = EHttpRequestType::DestroyEquipments;
		RequestParams.MemberID = PokeCore::DeviceId;
		RequestParams.DestoryEquipmentIds = DestroyEquipmentIds;

		HttpActor->Request(RequestParams);
	}
}

//void APokeCollectionCharacter::DeleteItemsByID(TMap<int32, int32>& InItemIDs)
//{
//	for (auto&& Item : InItemIDs)
//	{
//		UPokeItem* HaveItem = GetItemByID(Item.Key);
//		if (HaveItem)
//		{
//			int32 NewStackNum = HaveItem->GetStackNum() - Item.Value;
//
//			if (NewStackNum <= 0)
//			{
//				int32 ItemID = Item.Key;
//				HaveItems.RemoveAll([ItemID](UPokeItem* PokeItem) { return PokeItem->GetItemID() == ItemID; });
//			}
//			else
//			{
//				HaveItem->SetStackNum(NewStackNum);
//			}
//		}
//	}
//}

void APokeCollectionCharacter::DeleteItemsByKey(TMap<int32, int32>& InItemKeys)
{
	AHttpActor* HttpActor = PokeCore::GetHttpActor(GetWorld());
	if (!ensure(HttpActor))
	{
		return;
	}

	for (auto&& Item : InItemKeys)
	{
		UPokeItem* HaveItem = GetItemByKey(Item.Key);
		if (HaveItem)
		{
			int32 NewStackNum = HaveItem->GetStackNum() - Item.Value;

			if (NewStackNum <= 0)
			{
				int32 ItemKey = Item.Key;
				HaveItems.RemoveAll([ItemKey](UPokeItem* PokeItem) { return PokeItem->GetItemKey() == ItemKey; });
			}
			else
			{
				HaveItem->SetStackNum(NewStackNum);
			}
		}
	}

	FHttpRequestParams RequestParams;
	RequestParams.RequestType = EHttpRequestType::DestroyItems;
	RequestParams.MemberID = PokeCore::DeviceId;
	RequestParams.DestoryItemKeys = InItemKeys;

	HttpActor->Request(RequestParams);
}

void APokeCollectionCharacter::UpdateCharacters(TArray<int32>& InCharacterIDs)
{
	if (InCharacterIDs.Num() <= 0)
	{
		return;
	}

	AHttpActor* HttpActor = PokeCore::GetHttpActor(GetWorld());
	if (!HttpActor)
	{
		return;
	}

	FHttpRequestParams Params;
	Params.RequestType = EHttpRequestType::UpdateCharacters;
	Params.MemberID = PokeCore::DeviceId;
	Params.UpdateCharacterIds = InCharacterIDs;

	HttpActor->Request(Params);
}

void APokeCollectionCharacter::UpdateEquipments(TArray<int32>& InEquipmentIDs)
{
	if (InEquipmentIDs.Num() <= 0)
	{
		return;
	}

	AHttpActor* HttpActor = PokeCore::GetHttpActor(GetWorld());
	if (!HttpActor)
	{
		return;
	}

	FHttpRequestParams Params;
	Params.RequestType = EHttpRequestType::UpdateEquipments;
	Params.MemberID = PokeCore::DeviceId;
	Params.UpdateEquipmentIds = InEquipmentIDs;

	HttpActor->Request(Params);
}

void APokeCollectionCharacter::GetReward(FBattleReward& InBattleReward)
{
	if (InBattleReward.ExperienceAmount > 0)
	{
		TArray<int32> UpdatedCharacterIds;

		const TMap<int32, APokeCharacter*>& CurrentPartyCharacters = GetPartyCharacters(CurrentSelectedParty);

		for (auto&& PartyMember : CurrentPartyCharacters)
		{
			APokeCharacter* PartyCharacter = PartyMember.Value;
			if (!PartyCharacter)
			{
				continue;
			}

			PartyCharacter->TakeExperience(InBattleReward.ExperienceAmount);
			UpdatedCharacterIds.Add(PartyCharacter->GetCharacterID());
		}

		UpdateCharacters(UpdatedCharacterIds);
	}

	// NewCharacters
	TArray<FInitCharacterParams> NewCharactersParams;
	for (int32 NewCharacterKey : InBattleReward.GetCharacters)
	{
		FInitCharacterParams Params;
		Params.CharacterKey = NewCharacterKey;

		NewCharactersParams.Add(Params);
	}
	AddNewCharacters(NewCharactersParams);

	// NewItems
	TArray<FInitItemParams> AddItems;
	for (auto&& NewItemInfo : InBattleReward.GetItems)
	{
		FInitItemParams Params;
		Params.ItemKey = NewItemInfo.Key;
		Params.ItemStackNum = NewItemInfo.Value;

		AddItems.Add(Params);
	}
	AddNewItems(AddItems);

	AddMoney(InBattleReward.MoneyAmount);
	SetPlayerExp(GetPlayerCurrentExp() + InBattleReward.ExperienceAmount);
}

void APokeCollectionCharacter::SetMaxOpenedChapterNum(int32 NewMaxOpenedChapterNum)
{
	MaxOpenedChapterNum = NewMaxOpenedChapterNum;

	SavePlayerInfo(ESavePlayerInfo::MaxOpenedChapter);
}

void APokeCollectionCharacter::SetMaxClearBattleStage(battleStageKey InBattleStageKey)
{
	MaxClearBattleStageNum = FMath::Max(MaxClearBattleStageNum, InBattleStageKey);

	if (MaxClearBattleStageNum != InBattleStageKey)
	{
		SavePlayerInfo(ESavePlayerInfo::MaxClearBattleStage);

		UPlayerQuestComponent* QuestComp = GetQuestComponent();
		if (QuestComp)
		{
			FUpdateQuestParams Params;
			Params.ObjectionType = EQuestObjectionType::ClearBattleStageNum;
			Params.bIsAdd = false;
			Params.InCount = MaxClearBattleStageNum;

			QuestComp->UpdateQuest(Params);
		}
	}
}

void APokeCollectionCharacter::SetCurrentSelectedPartyNum(int32 NewSelectedPartyNum)
{
	CurrentSelectedParty = NewSelectedPartyNum;

	SavePlayerInfo(ESavePlayerInfo::LastSelectedParty);
}

void APokeCollectionCharacter::SetMaxHaveCharactersNum(int32 NewMaxHaveCharactersNum)
{
	MaxHaveCharactersNum = NewMaxHaveCharactersNum;

	SavePlayerInfo(ESavePlayerInfo::MaxHaveCharactersNum);
}

void APokeCollectionCharacter::SetMaxHaveEquipmentsNum(int32 NewMaxHaveEquipmentsNum)
{
	MaxHaveEquipmentsNum = NewMaxHaveEquipmentsNum;

	SavePlayerInfo(ESavePlayerInfo::MaxHaveEquipmentsNum);
}

void APokeCollectionCharacter::PutOnEquipment(int32 InCharacterID, int32 InEquipmentID)
{
	TArray<int32> UpdatedEquipIds;

	APokeCharacter* PokeCharacter = GetCharacterByID(InCharacterID);
	UPokeEquipment* NextEquipment = GetEquipmentByID(InEquipmentID);

	if (NextEquipment)
	{
		UpdatedEquipIds.Add(NextEquipment->GetEquipmentID());
	}

	if (PokeCharacter)
	{
		UPokeEquipment* PrevEquipment = PokeCharacter->GetCurrentEquipment();
		if (PrevEquipment)
		{
			UpdatedEquipIds.Add(PrevEquipment->GetEquipmentID());
		}
	}

	if (PokeCharacter && !NextEquipment)
	{
		PokeCharacter->TakeOffEquipment();
		UpdateEquipments(UpdatedEquipIds);
		return;
	}


	if (NextEquipment && !PokeCharacter)
	{
		NextEquipment->SetOwnerCharacterID(-1);
		UpdateEquipments(UpdatedEquipIds);
		return;
	}

	if (!NextEquipment || !PokeCharacter)
	{
		return;
	}

	if (NextEquipment->GetOwnerCharacterID() >= 0)
	{
		APokeCharacter* PrevCharacter = GetCharacterByID(NextEquipment->GetOwnerCharacterID());
		if (ensure(PrevCharacter))
		{
			PrevCharacter->TakeOffEquipment();
		}
	}

	PokeCharacter->PutOnEquipment(NextEquipment);
	
	UpdateEquipments(UpdatedEquipIds);
}

void APokeCollectionCharacter::TakeOffEquipment(int32 InCharacterID)
{
	TArray<int32> UpdatedEquipIds;

	APokeCharacter* PokeCharacter = GetCharacterByID(InCharacterID);
	if (PokeCharacter)
	{
		UPokeEquipment* PrevEquipment = PokeCharacter->GetCurrentEquipment();
		if (PrevEquipment)
		{
			UpdatedEquipIds.Add(PrevEquipment->GetEquipmentID());

			PokeCharacter->TakeOffEquipment();
		}
	}

	UpdateEquipments(UpdatedEquipIds);
}

bool APokeCollectionCharacter::SaveQuests()
{
	if (!QuestComponent)
	{
		return false;
	}

	QuestComponent->SaveQuests();

	return true;
}

void APokeCollectionCharacter::SetPlayerNickName(FString& InNickname)
{
	PlayerNickName = FName(*InNickname);

	SavePlayerInfo(ESavePlayerInfo::Nickname);
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

	SavePlayerInfo(ESavePlayerInfo::Level);
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

void APokeCollectionCharacter::SetCurrentSelectedChapterNum(int32 NewSelectedChapterNum)
{
	CurrentSelectedChapterNum = FMath::Clamp(NewSelectedChapterNum, 1, MaxOpenedChapterNum);

	SavePlayerInfo(ESavePlayerInfo::LastSelectedChapter);
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

	SavePlayerInfo(ESavePlayerInfo::BattleSpeed);
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

	UPlayerQuestComponent* QuestComp = GetQuestComponent();
	if (QuestComp)
	{
		FUpdateQuestParams Params;
		Params.ObjectionType = EQuestObjectionType::SpendBerryAmount;
		Params.InCount = InConsumeBerryAmount;

		QuestComp->UpdateQuest(Params);
	}
}

void APokeCollectionCharacter::AddMoney(int32 InAddAmout)
{
	if (InAddAmout > 0)
	{
		SetMoneyAmount(GetMoneyAmount() + InAddAmout);
	}
}

void APokeCollectionCharacter::SetMoneyAmount(int32 NewMoneyAmount)
{
	MoneyAmount = FMath::Clamp(NewMoneyAmount, 0, INT32_MAX);

	SavePlayerInfo(ESavePlayerInfo::Money);
}

void APokeCollectionCharacter::SpendMoney(int32 InSpendAmout)
{
	SetMoneyAmount(GetMoneyAmount() - InSpendAmout);

	UPlayerQuestComponent* QuestComp = GetQuestComponent();
	if (QuestComp)
	{
		FUpdateQuestParams Params;
		Params.ObjectionType = EQuestObjectionType::SpendMoneyAmount;
		Params.InCount = InSpendAmout;

		QuestComp->UpdateQuest(Params);
	}
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
	HttpActor->OnHttpHaveItemsResponseReceived.BindUObject(this, &APokeCollectionCharacter::OnHaveItemsResponsed);

	FHttpRequestParams Params;
	Params.RequestType = EHttpRequestType::Login;
	Params.MemberID = PokeCore::DeviceId;

	HttpActor->Request(Params);

	InitMainCharacter();
	//InitHaveEquipments();
	//InitHaveItems();

	//PutOnEquipment(1, 1);

	/*Params.RequestType = EHttpRequestType::RegistQuests;

	HttpActor->Request(Params);*/
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

void APokeCollectionCharacter::SetIndex(FString& NewIndex)
{
	for (int32 StringIndex = 0; StringIndex < NewIndex.Len(); ++StringIndex)
	{
		if (NewIndex[StringIndex] == '1')
		{
			if (CharacterIndex.Contains(StringIndex + 1))
			{
				CharacterIndex[StringIndex + 1] = true;
			}
		}
	}
}

FString APokeCollectionCharacter::GetIndex()
{
	FString PlayerIndex;

	for (auto&& Index : CharacterIndex)
	{
		PlayerIndex += (Index.Value) ? TEXT("1") : TEXT("0");
	}

	return PlayerIndex;
}

void APokeCollectionCharacter::TickResourceCharge(float DeltaSeconds)
{
	BerryChargingIntervalAgeSeconds += DeltaSeconds;
	MoneyChargingIntervalAgeSeconds += DeltaSeconds;

	while (BerryChargingIntervalAgeSeconds >= BerryChargingIntervalSeconds)
	{
		BerryChargingIntervalAgeSeconds -= BerryChargingIntervalSeconds;
		SetBerryAmount(BerryAmount + BerryChargingAmount);
		//BerryAmount += BerryChargingAmount;
	}

	while (MoneyChargingIntervalAgeSeconds >= MoneyChargingIntervalSeconds)
	{
		MoneyChargingIntervalAgeSeconds -= MoneyChargingIntervalSeconds;
		SetMoneyAmount(MoneyAmount + MoneyChargingAmount);
		//MoneyAmount += MoneyChargingAmount;
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
		TSharedPtr<FJsonObject> PlayerData = ResponceJson->GetObjectField("data");

		FInitPlayerParams Params;
		Params.PlayerNickname = PlayerData->GetStringField("nickname");
		Params.PlayerLevel = PlayerData->GetIntegerField("level");
		Params.PlayerCurrentExp = PlayerData->GetIntegerField("exp");
		Params.PlayerMoney = PlayerData->GetIntegerField("money");
		Params.PlayerBerry = PlayerData->GetIntegerField("berry");
		Params.PlayerStardust = PlayerData->GetIntegerField("stardust");
		Params.MaxClearBattleStage = PlayerData->GetIntegerField("maxClearBattleStage");
		Params.LastSelectedChapter = PlayerData->GetIntegerField("lastSelectedChapter");
		Params.MaxOpenedChapter = PlayerData->GetIntegerField("maxOpenedChapter");
		Params.LastSelectedParty = PlayerData->GetIntegerField("lastSelectedParty");
		Params.BattleClearCount = PlayerData->GetIntegerField("battleClearCount");
		Params.GetCharacterCount = PlayerData->GetIntegerField("getCharacterCount");
		Params.MaxHaveCharactersNum = PlayerData->GetIntegerField("maxHaveCharactersNum");
		Params.MaxHaveEquipmentsNum = PlayerData->GetIntegerField("maxHaveEquipmentsNum");
		Params.Index = PlayerData->GetStringField("index");
		Params.BattleSpeed = PlayerData->GetIntegerField("battleSpeed");

		InitPlayerInfo(Params);
	}
	else
	{
		// 로그인 실패
		ensure(0);
	}

	InitHaveCharacters();
}

void APokeCollectionCharacter::OnHaveCharactersResponsed(FHttpRequestPtr Request, TSharedPtr<FJsonObject> ResponceJson, bool bWasSuccessful)
{
	TArray<TSharedPtr<FJsonValue>> Characters = ResponceJson->GetArrayField("data");

	for (int32 Index = 0; Index < Characters.Num(); ++Index)
	{
		TSharedPtr<FJsonObject> CharacterJs = Characters[Index]->AsObject();

		FString MemberId = CharacterJs->GetStringField("memberId");

		FInitCharacterParams InitCharacterParams;
		InitCharacterParams.CharacterID = CharacterJs->GetIntegerField("characterId");
		InitCharacterParams.CharacterKey = CharacterJs->GetIntegerField("characterKey");
		InitCharacterParams.CharacterLevel = CharacterJs->GetIntegerField("characterLevel");
		InitCharacterParams.CurrentExp = CharacterJs->GetIntegerField("characterExp");
		InitCharacterParams.EvHealth = CharacterJs->GetIntegerField("EvHealth");
		InitCharacterParams.EvAttack = CharacterJs->GetIntegerField("EvAttack");
		InitCharacterParams.EvDefence = CharacterJs->GetIntegerField("EvDefense");
		InitCharacterParams.EvSpecialAttack = CharacterJs->GetIntegerField("EvSpAttack");
		InitCharacterParams.EvSpecialDefence = CharacterJs->GetIntegerField("EvSpDefense");
		InitCharacterParams.EvSpeed = CharacterJs->GetIntegerField("EvSpeed");
		InitCharacterParams.JoinedPartyNum = CharacterJs->GetIntegerField("joinedParty");
		InitCharacterParams.JoinedSlotNum = CharacterJs->GetIntegerField("joinedSlot");
		InitCharacterParams.Skill1Level = CharacterJs->GetIntegerField("skill1Level");
		InitCharacterParams.Skill2Level = CharacterJs->GetIntegerField("skill2Level");
		InitCharacterParams.Skill3Level = CharacterJs->GetIntegerField("skill3Level");
		InitCharacterParams.Skill4Level = CharacterJs->GetIntegerField("skill4Level");

		APokeCharacter* PokeCharacter = NewObject<APokeCharacter>();
		if (PokeCharacter)
		{
			PokeCharacter->Init(InitCharacterParams);

			HaveCharacters.AddUnique(PokeCharacter);
		}

	}

	InitHaveEquipments();
}

void APokeCollectionCharacter::OnHaveEquipmentsResponsed(FHttpRequestPtr Request, TSharedPtr<FJsonObject> ResponceJson, bool bWasSuccessful)
{
	TArray<TSharedPtr<FJsonValue>> Equipments = ResponceJson->GetArrayField("data");

	for (int32 Index = 0; Index < Equipments.Num(); ++Index)
	{
		TSharedPtr<FJsonObject> EquipmentJs = Equipments[Index]->AsObject();

		FString MemberId = EquipmentJs->GetStringField("memberId");

		FInitEquipmentParams InitEquipmentParams;
		InitEquipmentParams.EquipmentID = EquipmentJs->GetIntegerField("equipmentId");
		InitEquipmentParams.EquipmentKey = EquipmentJs->GetIntegerField("equipmentKey");
		InitEquipmentParams.EquipmentLevel = EquipmentJs->GetIntegerField("equipmentLevel");
		InitEquipmentParams.OwnerCharacterID = EquipmentJs->GetIntegerField("ownerCharacterId");

		UPokeEquipment* PokeEquipment = NewObject<UPokeEquipment>();
		if (PokeEquipment)
		{
			PokeEquipment->Init(InitEquipmentParams);

			HaveEquipments.AddUnique(PokeEquipment);

			if (InitEquipmentParams.OwnerCharacterID >= 0)
			{
				TArray<int32> UpdatedEquipIds;

				APokeCharacter* PokeCharacter = GetCharacterByID(InitEquipmentParams.OwnerCharacterID);

				if (!PokeCharacter)
				{
					PokeEquipment->SetOwnerCharacterID(-1);
				}
				else
				{
					PokeCharacter->PutOnEquipment(PokeEquipment);
				}
			}
		}
	}

	InitHaveItems();
}

void APokeCollectionCharacter::OnHaveItemsResponsed(FHttpRequestPtr Request, TSharedPtr<FJsonObject> ResponceJson, bool bWasSuccessful)
{
	TArray<TSharedPtr<FJsonValue>> Items = ResponceJson->GetArrayField("data");

	for (int32 Index = 0; Index < Items.Num(); ++Index)
	{
		TSharedPtr<FJsonObject> ItemJs = Items[Index]->AsObject();

		FString MemberId = ItemJs->GetStringField("memberId");

		FInitItemParams InitItemParams;
		InitItemParams.ItemID = ItemJs->GetIntegerField("itemId");
		InitItemParams.ItemKey = ItemJs->GetIntegerField("itemKey");
		InitItemParams.ItemStackNum = ItemJs->GetIntegerField("stackNum");
		
		UPokeItem* PokeItem = NewObject<UPokeItem>();
		if (PokeItem)
		{
			PokeItem->Init(InitItemParams);

			HaveItems.AddUnique(PokeItem);
		}

	}

	InitHaveQuests();
}

void APokeCollectionCharacter::SavePlayerInfo(ESavePlayerInfo InSaveInfo)
{
	AHttpActor* HttpActor = PokeCore::GetHttpActor(GetWorld());
	if (!ensure(HttpActor))
	{
		return;
	}

	FInitPlayerParams Params;
	Params.PlayerNickname = PlayerNickName.ToString();
	Params.PlayerLevel = PlayerLevel;
	Params.PlayerCurrentExp = PlayerCurrentExp;
	Params.PlayerMoney = MoneyAmount;
	Params.PlayerBerry = BerryAmount;
	Params.PlayerStardust = StardustAmount;
	Params.MaxClearBattleStage = MaxClearBattleStageNum;
	Params.LastSelectedChapter = CurrentSelectedChapterNum;
	Params.MaxOpenedChapter = MaxOpenedChapterNum;
	Params.LastSelectedParty = CurrentSelectedParty;
	Params.BattleClearCount = BattleClearCount;
	Params.GetCharacterCount = GetCharacterCount;
	Params.MaxHaveCharactersNum = MaxHaveCharactersNum;
	Params.MaxHaveEquipmentsNum = MaxHaveEquipmentsNum;
	Params.Index = GetIndex();
	Params.BattleSpeed = BattleSpeedMultiplier;

	FHttpRequestParams RequestParams;
	RequestParams.RequestType = EHttpRequestType::SavePlayerInfo;
	RequestParams.MemberID = PokeCore::DeviceId;
	RequestParams.SaveColumn = InSaveInfo;
	RequestParams.SavePlayerInfos = Params;

	HttpActor->Request(RequestParams);
}
