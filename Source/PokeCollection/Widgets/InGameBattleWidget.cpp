// Copyright Me. All Rights Reserved.


#include "InGameBattleWidget.h"

#include "Button.h"
#include "Components/ScrollBox.h"
#include "HorizontalBox.h"
#include "HorizontalBoxSlot.h"
#include "Image.h"
#include "ProgressBar.h"
#include "TextBlock.h"
#include "WidgetTree.h"

#include "BattleCharacterActor.h"
#include "BattleCharacterCombatComponent.h"
#include "BattleCharacterHealthComponent.h"
#include "BattleManager.h"
#include "PokeCore.h"
#include "PokeCollectionCharacter.h"
#include "PokeSkill.h"

void UInGameBattleWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SkillSlotBox->ClearChildren();

	if (!ensure(CharacterSkillSlotClass.Get()))
	{
		return;
	}

	for (int32 SlotNum = 0; SlotNum < 6; ++SlotNum)
	{
		UBattleCharacterSkillSlot* SkillSlot = CreateWidget<UBattleCharacterSkillSlot>(GetWorld(), CharacterSkillSlotClass.Get());
		SkillSlot->SetVisibility(ESlateVisibility::Collapsed);

		UHorizontalBoxSlot* BoxSlot = SkillSlotBox->AddChildToHorizontalBox(SkillSlot);
		/*if (ensure(BoxSlot))
		{
			BoxSlot->SetSize(FSlateChildSize());
			BoxSlot->SetHorizontalAlignment(HAlign_Center);
			BoxSlot->SetVerticalAlignment(VAlign_Center);
		}*/
	}

	if (LogWidget)
	{
		LogWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (LogButton)
	{
		LogButton->OnClicked.AddUniqueDynamic(this, &UInGameBattleWidget::OnLogButtonClicked);
	}

	if (ChangeBattleSpeedButton)
	{
		ChangeBattleSpeedButton->OnClicked.AddUniqueDynamic(this, &UInGameBattleWidget::OnChangeBattleSpeedButtonClicked);
	}

	if (BattleSpeedText)
	{
		APokeCollectionCharacter* Player = Cast<APokeCollectionCharacter>(GetOwningPlayerPawn());
		if (!Player)
		{
			return;
		}

		BattleSpeedText->SetText(FText::FromString(FString::FromInt(Player->GetBattleSpeedMultiplier())));
	}
}

void UInGameBattleWidget::InitBattleCharacters(const TArray<class ABattleCharacterActor*>& InPlayerBattleCharacter)
{
	PlayerBattleCharacters.Empty();
	OnBattleSpeedChange.Clear();

	PlayerBattleCharacters = InPlayerBattleCharacter;

	for (int32 Index = 0; Index < PlayerBattleCharacters.Num(); ++Index)
	{
		if (PlayerBattleCharacters.IsValidIndex(Index))
		{
			UBattleCharacterSkillSlot* SkillSlot = Cast<UBattleCharacterSkillSlot>(SkillSlotBox->GetChildAt(Index));
			if (!ensure(SkillSlot))
			{
				continue;
			}

			SkillSlot->SetOwnerBattleCharacter(*PlayerBattleCharacters[Index]);
			SkillSlot->SetVisibility(ESlateVisibility::Visible);

			OnBattleSpeedChange.AddUniqueDynamic(PlayerBattleCharacters[Index], &ABattleCharacterActor::OnBattleSpeedChange);
		}
	}
}

void UInGameBattleWidget::OnLogButtonClicked()
{
	if (!ensure(LogWidget))
	{
		return;
	}

	if (LogWidget->IsVisible())
	{
		LogWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		LogWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UInGameBattleWidget::OnChangeBattleSpeedButtonClicked()
{
	APokeCollectionCharacter* Player = Cast<APokeCollectionCharacter>(GetOwningPlayerPawn());
	if (!Player)
	{
		return;
	}

	Player->ChangeBattleSpeedMultiplier();

	if (BattleSpeedText)
	{
		BattleSpeedText->SetText(FText::FromString(FString::FromInt(Player->GetBattleSpeedMultiplier())));
	}

	OnBattleSpeedChange.Broadcast(Player->GetBattleSpeedMultiplier());

	UE_LOG(LogTemp, Log, TEXT("Battle Speed: %d"), Player->GetBattleSpeedMultiplier());
}


void UBattleCharacterSkillSlot::SetOwnerBattleCharacter(class ABattleCharacterActor& InBattleCharacter)
{
	OwnerBattleCharacter = &InBattleCharacter;

	InitSlotInfo();
}

void UBattleCharacterSkillSlot::OnUseSkillButtonClicked()
{
	if (!ActiveSkill)
	{
		return;
	}

	if (!OwnerBattleCharacter || OwnerBattleCharacter->IsDead())
	{
		return;
	}

	if (ActiveSkill->CanUseSkill())
	{
		FPokeUseSkillParams Params;

		UBattleCharacterCombatComponent* CombatComp = OwnerBattleCharacter->GetCombatComponent();
		if (!CombatComp)
		{
			return;
		}

		Params.TargetCharacter = CombatComp->GetCurrentTargetCharacter();

		if (!Params.TargetCharacter)
		{
			ABattleManager* BattleManager = PokeCore::GetBattleManager(GetWorld());
			if (BattleManager)
			{
				const TArray<class ABattleCharacterActor*> Enemies = BattleManager->GetBattleCharacters(true);
				for (auto&& Enemy : Enemies)
				{
					if (!Enemy || Enemy->IsDead())
					{
						continue;
					}

					Params.TargetCharacter = Enemy;
					break;
				}
			}
		}

		CombatComp->UseActiveSkill(Params);
	}
}

void UBattleCharacterSkillSlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (UseSkillButton)
	{
		UseSkillButton->OnClicked.AddUniqueDynamic(this, &UBattleCharacterSkillSlot::OnUseSkillButtonClicked);
	}
}

void UBattleCharacterSkillSlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!ensure(OwnerBattleCharacter))
	{
		return;
	}

	if (HealthBar)
	{
		UBattleCharacterHealthComponent* HealthComp = OwnerBattleCharacter->GetHealthComponent();
		if (HealthComp)
		{
			float HealthPoint = HealthComp->GetHealthPoint();
			float MaxHealthPoint = HealthComp->GetMaxHealthPoint();

			HealthBar->SetPercent(HealthPoint / MaxHealthPoint);
		}
	}

	if (SkillGaugeBar)
	{
		if (!ActiveSkill)
		{
			return;
		}

		float AttackCount = ActiveSkill->GetCurrentAttackCount();
		float MaxAttackCount = ActiveSkill->GetMaxAttackCount();

		SkillGaugeBar->SetPercent(AttackCount / MaxAttackCount);
	}

	if (SkillReady)
	{
		if (!ActiveSkill)
		{
			SkillReady->SetVisibility(ESlateVisibility::Hidden);
			return;
		}

		SkillReady->SetVisibility(ActiveSkill->CanUseSkill() ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	}
}

void UBattleCharacterSkillSlot::InitSlotInfo()
{
	if (!ensure(OwnerBattleCharacter))
	{
		return;
	}

	if (CharacterIcon)
	{
		CharacterIcon->SetBrushFromTexture(OwnerBattleCharacter->GetBattleProfile());
	}

	const TArray<class UPokeSkill*>& Skills = OwnerBattleCharacter->GetSkills();

	if (!Skills.IsValidIndex(3))
	{
		//ensure(0);
		return;
	}

	ActiveSkill = Skills[3];
}

void UBattleLogWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (BattleLogBox)
	{
		BattleLogBox->ClearChildren();
	}

	ABattleManager* BattleManager = PokeCore::GetBattleManager(GetWorld());
	if (BattleManager)
	{
		BattleManager->OnBattleLogAdded.AddUniqueDynamic(this, &UBattleLogWidget::OnBattleLogAdded);
	}
}

void UBattleLogWidget::OnBattleLogAdded(FString& NewBattleLog)
{
	if (!BattleLogBox)
	{
		return;
	}

	UTextBlock* LogText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
	if (ensure(LogText))
	{
		LogText->SetText(FText::FromString(NewBattleLog));
		BattleLogBox->AddChild(LogText);
	}

	BattleLogBox->ScrollToEnd();
}
