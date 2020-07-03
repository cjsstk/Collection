// Copyright Me. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/InGameCategoryWidget.h"
#include "QuestContentWidget.generated.h"


UCLASS()
class POKECOLLECTION_API UQuestSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void InitQuest(TWeakObjectPtr<class UPokeQuest> InWeakQuest);

	UFUNCTION()
	void OnCompleteButtonClicked();

private:
	void SetQuestKey(int32 InQuestKey) { QuestKey = InQuestKey; }
	void SetQuestDesc(const FText& InDesc);
	void SetQuestCount(int32 InCurrCount, int32 InDestCount);

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* QuestDescText = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* QuestCountText = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* QuestProgressBar = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* RewardScrollBox = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* CompleteButton = nullptr;

	TWeakObjectPtr<class UPokeQuest> WeakQuest = nullptr;

	int32 QuestKey = 0;
};


UCLASS()
class POKECOLLECTION_API UDailyMissionContentWidget : public UCategoryContentWidget
{
	GENERATED_BODY()

public:
	virtual void OnOpen() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* DailyMissionScrollBox = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UQuestSlot> DailyMissionSlotClass;

};


UCLASS()
class POKECOLLECTION_API UQuestContentWidget : public UCategoryContentWidget
{
	GENERATED_BODY()
	
public:
	virtual void OnOpen() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* QuestScrollBox = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UQuestSlot> QuestSlotClass;
};
