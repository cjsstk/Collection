// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentInfoPopUp.h"

#include "PokeCollectionCharacter.h"
#include "PokeEquipment.h"

#include "Image.h"
#include "TextBlock.h"
#include "Button.h"

void UEquipmentInfoPopUp::NativeConstruct()
{
	Super::NativeConstruct();

	if (BackgroundButton)
	{
		BackgroundButton->OnClicked.AddUniqueDynamic(this, &UEquipmentInfoPopUp::OnBackgroundClicked);
	}
}

void UEquipmentInfoPopUp::InitInfo(int32 InEquipmentID)
{
	APokeCollectionCharacter* Player = Cast<APokeCollectionCharacter>(GetOwningPlayerPawn());
	if (!Player)
	{
		return;
	}

	UPokeEquipment* Equipment = Player->GetEquipmentByID(InEquipmentID);
	if (!Equipment)
	{
		return;
	}

	if (EquipmentImage)
	{
		EquipmentImage->SetBrushFromTexture(Equipment->GetEquipmentProfileImage());
	}

	if (EquipmentName)
	{
		EquipmentName->SetText(FText::FromName(Equipment->GetEquipmentName()));
	}

	if (EquipmentDesc)
	{
		EquipmentDesc->SetText(Equipment->GetEquipmentDesc());
	}
}

void UEquipmentInfoPopUp::OnBackgroundClicked()
{
	RemoveFromViewport();
}
