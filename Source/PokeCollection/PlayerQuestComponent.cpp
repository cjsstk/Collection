// Copyright Me. All Rights Reserved.

#include "PlayerQuestComponent.h"

#include "CMS.h"
#include "PokeCore.h"
#include "PokeQuest.h"
#include "Net/HttpActor.h"

UPlayerQuestComponent::UPlayerQuestComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerQuestComponent::UpdateQuest(const FUpdateQuestParams& InUpdateParams)
{
	for (auto&& Quest : Quests)
	{
		UPokeQuest* PokeQuest = Quest.Value;
		if (!PokeQuest)
		{
			continue;
		}

		if (PokeQuest->CanComplete() || PokeQuest->GetObjectionType() != InUpdateParams.ObjectionType)
		{
			continue;
		}

		PokeQuest->Update(InUpdateParams.bIsAdd, InUpdateParams.InCount);
	}

	for (auto&& DailyMission : DailyMissions)
	{
		UPokeQuest* PokeMission = DailyMission.Value;
		if (!PokeMission)
		{
			continue;
		}

		if (PokeMission->CanComplete() || PokeMission->GetObjectionType() != InUpdateParams.ObjectionType)
		{
			continue;
		}

		PokeMission->Update(InUpdateParams.bIsAdd, InUpdateParams.InCount);
	}
}

void UPlayerQuestComponent::SaveQuests()
{
	AHttpActor* HttpActor = PokeCore::GetHttpActor(GetWorld());
	if (!HttpActor)
	{
		return;
	}

	TArray<FSaveQuestParams> SaveQuests;

	for (auto&& Quest : Quests)
	{
		UPokeQuest* PokeQuest = Quest.Value;
		if (!PokeQuest)
		{
			continue;
		}

		FSaveQuestParams Params;
		Params.QuestKey = Quest.Key;
		Params.CurrentNum = PokeQuest->GetCurrentCount();
		Params.bIsCompleted = PokeQuest->IsCompleted();

		SaveQuests.Add(Params);
	}

	FHttpRequestParams Params;
	Params.RequestType = EHttpRequestType::SaveQuests;
	Params.MemberID = PokeCore::DeviceId;
	Params.SaveQuestInfos = SaveQuests;

	HttpActor->Request(Params);

}

void UPlayerQuestComponent::BeginPlay()
{
	Super::BeginPlay();

	// Init Quests
	TArray<FQuestInfo*> QuestInfos;
	CMS::GetAllQuestDataTable(QuestInfos);

	for (int32 Index = 0; Index < QuestInfos.Num(); ++Index)
	{
		int32 QuestKey = Index + 1;

		if (IsCompletedQuest(QuestKey) || QuestInfos[Index]->ParentQuestKey > 0)
		{
			continue;
		}

		UPokeQuest* Quest = NewObject<UPokeQuest>();
		if (Quest)
		{
			FInitQuestParams Params;
			Params.QuestKey = QuestKey;
			Params.ObjectionType = QuestInfos[Index]->ObjectionType;
			Params.DestNum = QuestInfos[Index]->DestNum;
			Params.ParentQuestKey = QuestInfos[Index]->ParentQuestKey;
			Params.QuestDesc = QuestInfos[Index]->QuestDesc;

			Quest->InitQuest(Params);
			Quests.Add(QuestKey, Quest);
		}
	}

	// Init Daily Missions
	CMS::GetAllDailyMissionDataTable(QuestInfos);

	for (int32 Index = 0; Index < QuestInfos.Num(); ++Index)
	{
		int32 DailyMissionKey = Index + 1;

		UPokeQuest* DailyMission = NewObject<UPokeQuest>();
		if (DailyMission)
		{
			FInitQuestParams Params;
			Params.QuestKey = DailyMissionKey;
			Params.ObjectionType = QuestInfos[Index]->ObjectionType;
			Params.DestNum = QuestInfos[Index]->DestNum;
			Params.ParentQuestKey = QuestInfos[Index]->ParentQuestKey;
			Params.QuestDesc = QuestInfos[Index]->QuestDesc;

			DailyMission->InitQuest(Params);
			DailyMissions.Add(DailyMissionKey, DailyMission);
		}
	}
}

void UPlayerQuestComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

bool UPlayerQuestComponent::IsCompletedQuest(int32 InQuestKey)
{
	return CompletedQuestKeys.Contains(InQuestKey);
}

bool UPlayerQuestComponent::IsCompletedMission(int32 InMissionKey)
{
	return CompletedDailyMissionKeys.Contains(InMissionKey);
}
