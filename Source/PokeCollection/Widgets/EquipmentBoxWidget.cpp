// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentBoxWidget.h"

#include "Image.h"
#include "UniformGridPanel.h"
#include "UniformGridSlot.h"
#include "ScrollBox.h"
#include "TextBlock.h"

#include "PokeCollectionCharacter.h"
#include "PokeCollectionHUD.h"
#include "PokeCharacter.h"
#include "PokeEquipment.h"

void UEquipmentBoxWidget::NativeConstruct()
{
	BoxContentType = EBoxContentType::Equipment;

	Super::NativeConstruct();
}

void UEquipmentBoxWidget::OnOpen()
{
	Super::OnOpen();

	SetFocus();

	RefreshSlot();
}

void UEquipmentBoxWidget::RefreshSlot()
{
	Super::RefreshSlot();

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
		UInBoxEquipmentSlot* BoxSlot = Cast<UInBoxEquipmentSlot>(GridPanel->GetChildAt(Index));
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

	if (OwnerCharacterImage)
	{
		OwnerCharacterImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UEquipmentSlot::InitByID(int32 InContentID)
{
	Super::InitByID(InContentID);

	APokeCollectionCharacter* Player = PokeCore::GetPokePlayer(GetWorld());
	if (!ensure(Player))
	{
		return;
	}

	UPokeEquipment* Equipment = Player->GetEquipmentByID(InContentID);
	if (!Equipment)
	{
		return;
	}

	SetContentLevel(Equipment->GetLevel());
	SetOwnerCharacterImage(InContentID);
}

void UEquipmentSlot::SetContentLevel(int32 InLevel)
{
	if (LevelText)
	{
		FString EquipmentLevelText = FString("+");
		EquipmentLevelText += FString::FromInt(InLevel);
		LevelText->SetText(FText::FromString(EquipmentLevelText));
	}
}

bool UEquipmentSlot::SetOwnerCharacterImage(int32 InContentID)
{
	if (!bUseOwnerCharacterImage)
	{
		return false;
	}

	if (!ensure(OwnerCharacterImage))
	{
		return false;
	}

	APokeCollectionCharacter* Player = PokeCore::GetPokePlayer(GetWorld());
	if (!ensure(Player))
	{
		return false;
	}

	UPokeEquipment* Equipment = Player->GetEquipmentByID(InContentID);
	if (!Equipment)
	{
		return false;
	}

	if (Equipment->GetOwnerCharacterID() >= 0)
	{
		APokeCharacter* Character = Player->GetCharacterByID(Equipment->GetOwnerCharacterID());
		if (Character)
		{
			OwnerCharacterImage->SetBrushFromTexture(Character->GetCharacterProfileImage());
			OwnerCharacterImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			return true;
		}
		else
		{
			// Character destroyed?
			Equipment->SetOwnerCharacterID(-1);
			OwnerCharacterImage->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else
	{
		OwnerCharacterImage->SetVisibility(ESlateVisibility::Hidden);
	}

	return false;
}

void UInBoxEquipmentSlot::NativeConstruct()
{
	Super::NativeConstruct();

	bUseOwnerCharacterImage = true;
}

void UInBoxEquipmentSlot::OnSelectButtonClicked()
{
	APokeCollectionHUD* PokeHud = GetOwningPlayer() ? Cast<APokeCollectionHUD>(GetOwningPlayer()->GetHUD()) : nullptr;
	if (!PokeHud)
	{
		return;
	}

	PokeHud->OpenEquipmentInfoPopUp(ContentID);
}
