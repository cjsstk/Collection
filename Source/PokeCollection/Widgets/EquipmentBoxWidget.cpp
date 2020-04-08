// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentBoxWidget.h"

#include "UniformGridPanel.h"
#include "UniformGridSlot.h"
#include "ScrollBox.h"

#include "PokeCollectionCharacter.h"
#include "PokeCollectionHUD.h"
#include "PokeEquipment.h"

void UEquipmentBoxWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UEquipmentBoxWidget::OnOpen()
{
	Super::OnOpen();

	SetFocus();

	APokeCollectionCharacter* Player = Cast<APokeCollectionCharacter>(GetOwningPlayerPawn());
	if (!ensure(Player))
	{
		return;
	}

	if (!GridPanel)
	{
		return;
	}

	if (!BoxSlotClass.Get())
	{
		return;
	}

	const TArray<UPokeEquipment*>& HaveEquipments = Player->GetHaveEquipments();

	const int32 SlotNum = GridPanel->GetChildrenCount();

	for (int32 Index = 0; Index < SlotNum; Index++)
	{
		UEquipmentSlot* BoxSlot = Cast<UEquipmentSlot>(GridPanel->GetChildAt(Index));
		if (BoxSlot)
		{
			if (HaveEquipments.IsValidIndex(Index))
			{
				UPokeEquipment* CurrentEquipment = HaveEquipments[Index];
				if (!ensure(CurrentEquipment))
				{
					continue;
				}

				BoxSlot->InitByID(CurrentEquipment->GetEquipmentID());
				BoxSlot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
			else
			{
				if ((Index / ColumnNum) > ((HaveEquipments.Num() - 1) / ColumnNum))
				{
					BoxSlot->SetVisibility(ESlateVisibility::Collapsed);
				}
				else
				{
					BoxSlot->SetVisibility(ESlateVisibility::Hidden);
				}
			}
		}

	}
}

void UEquipmentSlot::NativeConstruct()
{
	Super::NativeConstruct();

	BoxContentType = EBoxContentType::Equipment;
}

void UEquipmentSlot::OnSelectButtonClicked()
{
	Super::OnSelectButtonClicked();

	APokeCollectionHUD* PokeHud = GetOwningPlayer() ? Cast<APokeCollectionHUD>(GetOwningPlayer()->GetHUD()) : nullptr;
	if (!PokeHud)
	{
		return;
	}

	PokeHud->OpenEquipmentInfoPopUp(ContentID);
}
