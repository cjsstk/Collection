// Copyright Me. All Rights Reserved.


#include "QuestContentWidget.h"

#include "Button.h"
#include "Components/ScrollBox.h"
#include "ProgressBar.h"
#include "TextBlock.h"

#include "CMS.h"
#include "PokeCollectionCharacter.h"
#include "PokeQuest.h"
#include "PlayerQuestComponent.h"

void UDailyMissionContentWidget::OnOpen()
{
	Super::OnOpen();

	if (!DailyMissionScrollBox)
	{
		return;
	}

	APokeCollectionCharacter* Player = Cast<APokeCollectionCharacter>(GetOwningPlayerPawn());
	if (!Player)
	{
		return;
	}

	UPlayerQuestComponent* QuestComponent = Player->GetQuestComponent();
	if (!QuestComponent)
	{
		return;
	}

	DailyMissionScrollBox->ClearChildren();

	const TMap<int32, class UPokeQuest*>& HaveMissions = QuestComponent->GetHaveDailyMissions();
	for (auto&& Mission : HaveMissions)
	{
		UPokeQuest* PokeMission = Mission.Value;
		if (!PokeMission)
		{
			continue;
		}

		UQuestSlot* QuestSlot = CreateWidget<UQuestSlot>(GetOwningPlayer(), DailyMissionSlotClass.Get());
		if (QuestSlot)
		{
			QuestSlot->InitQuest(PokeMission);

			DailyMissionScrollBox->AddChild(QuestSlot);
		}
	}

}

void UQuestContentWidget::OnOpen()
{
	Super::OnOpen();

	if (!QuestScrollBox)
	{
		return;
	}

	APokeCollectionCharacter* Player = Cast<APokeCollectionCharacter>(GetOwningPlayerPawn());
	if (!Player)
	{
		return;
	}

	UPlayerQuestComponent* QuestComponent = Player->GetQuestComponent();
	if (!QuestComponent)
	{
		return;
	}

	QuestScrollBox->ClearChildren();

	const TMap<int32, class UPokeQuest*>& HaveQuests = QuestComponent->GetHaveQuests();
	for (auto&& Quest : HaveQuests)
	{
		UPokeQuest* PokeQuest = Quest.Value;
		if (!PokeQuest)
		{
			continue;
		}

		UQuestSlot* QuestSlot = CreateWidget<UQuestSlot>(GetOwningPlayer(), QuestSlotClass.Get());
		if (QuestSlot)
		{
			QuestSlot->InitQuest(PokeQuest);

			QuestScrollBox->AddChild(QuestSlot);
		}
	}
}

void UQuestSlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (CompleteButton)
	{
		CompleteButton->OnClicked.AddUniqueDynamic(this, &UQuestSlot::OnCompleteButtonClicked);
	}
}

void UQuestSlot::InitQuest(TWeakObjectPtr<class UPokeQuest> InWeakQuest)
{
	WeakQuest = InWeakQuest;

	UPokeQuest* Quest = WeakQuest.Get();
	if (!ensure(Quest))
	{
		return;
	}

	SetQuestKey(Quest->GetQuestKey());
	SetQuestDesc(Quest->GetQuestDesc());
	SetQuestCount(Quest->GetCurrentCount(), Quest->GetDestCount());

	if (CompleteButton)
	{
		CompleteButton->SetIsEnabled(Quest->CanComplete());
	}
}

void UQuestSlot::SetQuestDesc(const FText& InDesc)
{
	if (QuestDescText)
	{
		QuestDescText->SetText(InDesc);
	}
}

void UQuestSlot::SetQuestCount(int32 InCurrCount, int32 InDestCount)
{
	if (QuestCountText)
	{
		FString CountText = FString::FromInt(InCurrCount) + TEXT("/") + FString::FromInt(InDestCount);

		QuestCountText->SetText(FText::AsCultureInvariant(CountText));
	}

	if (QuestProgressBar)
	{
		float Percent = (float)InCurrCount / (float)InDestCount;
		QuestProgressBar->SetPercent(Percent);
	}
}

void UQuestSlot::OnCompleteButtonClicked()
{
	APokeCollectionCharacter* Player = Cast<APokeCollectionCharacter>(GetOwningPlayerPawn());
	if (!Player)
	{
		return;
	}

	UPokeQuest* Quest = WeakQuest.Get();
	if (!ensure(Quest))
	{
		return;
	}

	if (!Quest->CanComplete())
	{
		return;
	}

	const FQuestInfo* QuestInfo = CMS::GetQuestDataTable(QuestKey);
	if (!QuestInfo)
	{
		return;
	}

	int32 RewardMoney = QuestInfo->RewardMoney;
	TMap<int32, int32> RewardItemKeys = QuestInfo->RewardItemKeys;

	FBattleReward Reward;
	Reward.GetItems = RewardItemKeys;
	Reward.MoneyAmount = RewardMoney;

	Player->GetReward(Reward);

	Player->SaveQuests();
}
