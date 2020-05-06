// Copyright Me. All Rights Reserved.


#include "EquipmentInfoContentWidget.h"

#include "PokeCharacter.h"
#include "PokeEquipment.h"

#include "Button.h"
#include "Image.h"
#include "VerticalBox.h"

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
	if (Equipment)
	{
		EquipmentImage->SetBrushFromTexture(Equipment->GetEquipmentProfileImage());
	}
	else
	{
		EquipmentImage->SetBrushFromTexture(EmptyEquipmentTexture);
	}

	StatusChangeBox->ClearChildren();

	const FStatus& ExcludeEquipmentStatus = SelectedCharacter->GetFinalStatus(false);
	int32 ExcludeEquipmentAttackRange = SelectedCharacter->GetAttackRange(false);
	const FEquipmentStatus& EquipmentStatus = Equipment->GetFinalEquipmentStatus();

	if (ensure(StatusChangeWidgetClass.Get()))
	{
		UStatusChangeByEquipmentWidget* AttackChangeWidget = CreateWidget<UStatusChangeByEquipmentWidget>(GetWorld(), StatusChangeWidgetClass.Get());
		if (AttackChangeWidget)
		{
			AttackChangeWidget->SetChangeStatusText(ExcludeEquipmentStatus.Attack, ExcludeEquipmentStatus.Attack + EquipmentStatus.Attack);
			StatusChangeBox->AddChildToVerticalBox(AttackChangeWidget);
		}

		UStatusChangeByEquipmentWidget* DefenseChangeWidget = CreateWidget<UStatusChangeByEquipmentWidget>(GetWorld(), StatusChangeWidgetClass.Get());
		if (DefenseChangeWidget)
		{
			DefenseChangeWidget->SetChangeStatusText(ExcludeEquipmentStatus.Defense, ExcludeEquipmentStatus.Defense + EquipmentStatus.Defense);
			StatusChangeBox->AddChildToVerticalBox(DefenseChangeWidget);
		}

		UStatusChangeByEquipmentWidget* SpecialAttackChangeWidget = CreateWidget<UStatusChangeByEquipmentWidget>(GetWorld(), StatusChangeWidgetClass.Get());
		if (SpecialAttackChangeWidget)
		{
			SpecialAttackChangeWidget->SetChangeStatusText(ExcludeEquipmentStatus.SpecialAttack, ExcludeEquipmentStatus.SpecialAttack + EquipmentStatus.SpecialAttack);
			StatusChangeBox->AddChildToVerticalBox(SpecialAttackChangeWidget);
		}

		UStatusChangeByEquipmentWidget* SpecialDefenseChangeWidget = CreateWidget<UStatusChangeByEquipmentWidget>(GetWorld(), StatusChangeWidgetClass.Get());
		if (SpecialDefenseChangeWidget)
		{
			SpecialDefenseChangeWidget->SetChangeStatusText(ExcludeEquipmentStatus.SpecialDefense, ExcludeEquipmentStatus.SpecialDefense + EquipmentStatus.SpecialDefense);
			StatusChangeBox->AddChildToVerticalBox(SpecialDefenseChangeWidget);
		}

		UStatusChangeByEquipmentWidget* SpeedChangeWidget = CreateWidget<UStatusChangeByEquipmentWidget>(GetWorld(), StatusChangeWidgetClass.Get());
		if (SpeedChangeWidget)
		{
			SpeedChangeWidget->SetChangeStatusText(ExcludeEquipmentStatus.Speed, ExcludeEquipmentStatus.Speed + EquipmentStatus.Speed);
			StatusChangeBox->AddChildToVerticalBox(SpeedChangeWidget);
		}

		UStatusChangeByEquipmentWidget* AttackRangeChangeWidget = CreateWidget<UStatusChangeByEquipmentWidget>(GetWorld(), StatusChangeWidgetClass.Get());
		if (AttackRangeChangeWidget)
		{
			AttackRangeChangeWidget->SetChangeStatusText(ExcludeEquipmentAttackRange, ExcludeEquipmentAttackRange + EquipmentStatus.AttackRange);
			StatusChangeBox->AddChildToVerticalBox(AttackRangeChangeWidget);
		}
	}
	
}

void UEquipmentInfoContentWidget::OnSlotButtonClicked()
{

}

void UStatusChangeByEquipmentWidget::SetChangeStatusText(int32 InExcludeStat, int32 InIncludeStat)
{

}
