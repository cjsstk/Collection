// Copyright Me. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CMSType.h"
#include "Net/HttpActor.h"
#include "PlayerQuestComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuestUpdate, EQuestObjectionType, ObjectionType, int32, Count);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class POKECOLLECTION_API UPlayerQuestComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerQuestComponent();

	void UpdateQuest(const FUpdateQuestParams& InUpdateParams);
	void SaveQuests();

	void OnHaveQuestsResponsed(FHttpRequestPtr Request, TSharedPtr<FJsonObject> ResponceJson, bool bWasSuccessful);

	const TMap<int32, class UPokeQuest*>& GetHaveQuests() const { return Quests; }
	const TMap<int32, class UPokeQuest*>& GetHaveDailyMissions() const { return DailyMissions; }

	FOnQuestUpdate OnQuestUpdate;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	bool IsCompletedQuest(int32 InQuestKey);
	bool IsCompletedMission(int32 InMissionKey);

	/** Key: questKey, Value: quest */
	UPROPERTY(Transient)
	TMap<int32, class UPokeQuest*> Quests;

	UPROPERTY(Transient)
	TMap<int32, class UPokeQuest*> DailyMissions;

	TArray<int32> CompletedQuestKeys;

	TArray<int32> CompletedDailyMissionKeys;
};
