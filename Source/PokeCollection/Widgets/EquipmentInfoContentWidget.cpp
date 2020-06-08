// Copyright Me. All Rights Reserved.


#include "EquipmentInfoContentWidget.h"

#include "PokeCollectionHUD.h"
#include "PokeCharacter.h"
#include "PokeEquipment.h"
#include "Widgets/EquipmentBoxWidget.h"

#include "Button.h"
#include "TextBlock.h"
#include "Image.h"
#include "VerticalBox.h"
#include "VerticalBoxSlot.h"

void UEquipmentInfoContentWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (SlotButton)
	{
		SlotButton->OnClicked.AddUniqueDynamic(this, &UEquipmentInfoContentWidget::OnSlotButtonClicked);
	}
}

void UEquipmentInfoContentWidget::OnOpen()
{
	Super::OnOpen();

	APokeCharacter* SelectedCharacter = GetSelectedCharacter();
	if (!ensure(SelectedCharacter))
	{
		return;
	}

	UPokeEquipment* Equipment = SelectedCharacter->GetCurrentEquipment();
	if (EquipmentSlot)
	{
		if (Equipment)
		{
			EquipmentSlot->InitByID(Equipment->GetEquipmentID());
		}
		else
		{
			EquipmentSlot->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	StatusChangeBox->ClearChildren();

	FStatus ExcludeEquipmentStatus = SelectedCharacter->GetFinalStatus(false);
	int32 ExcludeEquipmentAttackRange = SelectedCharacter->GetAttackRange(false);
	FEquipmentStatus EquipmentStatus = Equipment ? Equipment->GetFinalEquipmentStatus() : FEquipmentStatus();

	if (ensure(StatusChangeWidgetClass.Get()))
	{
		UStatusChangeByEquipmentWidget* AttackChangeWidget = CreateWidget<UStatusChangeByEquipmentWidget>(GetWorld(), StatusChangeWidgetClass.Get());
		if (AttackChangeWidget)
		{
			AttackChangeWidget->SetChangeStatusText(FString(TEXT("공격")), ExcludeEquipmentStatus.Attack, ExcludeEquipmentStatus.Attack + EquipmentStatus.Attack);

			UVerticalBoxSlot* BoxSlot = StatusChangeBox->AddChildToVerticalBox(AttackChangeWidget);
			if (BoxSlot)
			{
				BoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
				BoxSlot->SetSize(FSlateChildSize());
			}
		}

		UStatusChangeByEquipmentWidget* DefenseChangeWidget = CreateWidget<UStatusChangeByEquipmentWidget>(GetWorld(), StatusChangeWidgetClass.Get());
		if (DefenseChangeWidget)
		{
			DefenseChangeWidget->SetChangeStatusText(FString(TEXT("방어")), ExcludeEquipmentStatus.Defense, ExcludeEquipmentStatus.Defense + EquipmentStatus.Defense);
			UVerticalBoxSlot* BoxSlot = StatusChangeBox->AddChildToVerticalBox(DefenseChangeWidget);
			if (BoxSlot)
			{
				BoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
				BoxSlot->SetSize(FSlateChildSize());
			}
		}

		UStatusChangeByEquipmentWidget* SpecialAttackChangeWidget = CreateWidget<UStatusChangeByEquipmentWidget>(GetWorld(), StatusChangeWidgetClass.Get());
		if (SpecialAttackChangeWidget)
		{
			SpecialAttackChangeWidget->SetChangeStatusText(FString(TEXT("특수공격")), ExcludeEquipmentStatus.SpecialAttack, ExcludeEquipmentStatus.SpecialAttack + EquipmentStatus.SpecialAttack);
			UVerticalBoxSlot* BoxSlot = StatusChangeBox->AddChildToVerticalBox(SpecialAttackChangeWidget);
			if (BoxSlot)
			{
				BoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
				BoxSlot->SetSize(FSlateChildSize());
			}
		}

		UStatusChangeByEquipmentWidget* SpecialDefenseChangeWidget = CreateWidget<UStatusChangeByEquipmentWidget>(GetWorld(), StatusChangeWidgetClass.Get());
		if (SpecialDefenseChangeWidget)
		{
			SpecialDefenseChangeWidget->SetChangeStatusText(FString(TEXT("특수방어")), ExcludeEquipmentStatus.SpecialDefense, ExcludeEquipmentStatus.SpecialDefense + EquipmentStatus.SpecialDefense);
			UVerticalBoxSlot* BoxSlot = StatusChangeBox->AddChildToVerticalBox(SpecialDefenseChangeWidget);
			if (BoxSlot)
			{
				BoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
				BoxSlot->SetSize(FSlateChildSize());
			}
		}

		UStatusChangeByEquipmentWidget* SpeedChangeWidget = CreateWidget<UStatusChangeByEquipmentWidget>(GetWorld(), StatusChangeWidgetClass.Get());
		if (SpeedChangeWidget)
		{
			SpeedChangeWidget->SetChangeStatusText(FString(TEXT("스피드")), ExcludeEquipmentStatus.Speed, ExcludeEquipmentStatus.Speed + EquipmentStatus.Speed);
			UVerticalBoxSlot* BoxSlot = StatusChangeBox->AddChildToVerticalBox(SpeedChangeWidget);
			if (BoxSlot)
			{
				BoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
				BoxSlot->SetSize(FSlateChildSize());
			}
		}

		UStatusChangeByEquipmentWidget* AttackRangeChangeWidget = CreateWidget<UStatusChangeByEquipmentWidget>(GetWorld(), StatusChangeWidgetClass.Get());
		if (AttackRangeChangeWidget)
		{
			AttackRangeChangeWidget->SetChangeStatusText(FString(TEXT("사거리")), ExcludeEquipmentAttackRange, ExcludeEquipmentAttackRange + EquipmentStatus.AttackRange);
			UVerticalBoxSlot* BoxSlot = StatusChangeBox->AddChildToVerticalBox(AttackRangeChangeWidget);
			if (BoxSlot)
			{
				BoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
				BoxSlot->SetSize(FSlateChildSize());
			}
		}
	}
	
}

void UEquipmentInfoContentWidget::OnSlotButtonClicked()
{
	APokeCollectionHUD* PokeHud = GetOwningPlayer() ? Cast<APokeCollectionHUD>(GetOwningPlayer()->GetHUD()) : nullptr;
	if (!PokeHud)
	{
		return;
	}

	APokeCharacter* SelectedCharacter = GetSelectedCharacter();
	if (!ensure(SelectedCharacter))
	{
		return;
	}

	UPokeEquipment* Equipment = SelectedCharacter->GetCurrentEquipment();
	if (Equipment)
	{
		PokeHud->OpenEquipmentInfoPopUp(Equipment->GetEquipmentID());
	}
	else
	{
		PokeHud->OpenInGameChangeEquipmentWidget();
	}
}

void UStatusChangeByEquipmentWidget::SetChangeStatusText(FString InStatusName, int32 InExcludeStat, int32 InIncludeStat)
{
	if (StatusNameText)
	{
		StatusNameText->SetText(FText::FromString(InStatusName + FString(": ")));
	}

	if (ExcludeEquipmentStatText)
	{
		ExcludeEquipmentStatText->SetText(FText::FromString(FString::FromInt(InExcludeStat)));
	}

	if (IncludeEquipmentStatText)
	{
		IncludeEquipmentStatText->SetText(FText::FromString(FString::FromInt(InIncludeStat)));
	}

	if (ChangeAmountText)
	{
		FString Plus = FString("(+");
		Plus += FString::FromInt(InIncludeStat - InExcludeStat);
		Plus += FString(")");
		ChangeAmountText->SetText(FText::FromString(Plus));
	}
}
