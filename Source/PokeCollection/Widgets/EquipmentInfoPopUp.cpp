// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentInfoPopUp.h"

#include "PokeCharacter.h"
#include "PokeCollectionCharacter.h"
#include "PokeCollectionHUD.h"
#include "PokeEquipment.h"
#include "Widgets/InGameCharacterInfoWidget.h"

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

	if (ExcludeButton)
	{
		ExcludeButton->OnClicked.AddUniqueDynamic(this, &UEquipmentInfoPopUp::OnExcludeButtonClicked);
	}

	if (ChangeButton)
	{
		ChangeButton->OnClicked.AddUniqueDynamic(this, &UEquipmentInfoPopUp::OnChangeButtonClicked);
	}

	if (UpgradeButton)
	{
		UpgradeButton->OnClicked.AddUniqueDynamic(this, &UEquipmentInfoPopUp::OnUpgradeButtonClicked);
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

	EquipmentID = InEquipmentID;

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

	APokeCollectionHUD* PokeHud = Cast<APokeCollectionHUD>(GetOwningPlayer()->GetHUD());
	if (!PokeHud)
	{
		return;
	}

	UInGameCharacterInfoWidget* InfoWidget = PokeHud->GetInGameCharacterInfoWidget();
	if (!InfoWidget)
	{
		return;
	}

	APokeCharacter* Character = Player->GetCharacterByID(InfoWidget->GetSelectedCharacterID());
	if (ExcludeButton)
	{
		ExcludeButton->SetIsEnabled(Character);
	}

	if (ChangeButton)
	{
		ChangeButton->SetIsEnabled(Character);
	}
}

void UEquipmentInfoPopUp::OnBackgroundClicked()
{
	RemoveFromViewport();
}

void UEquipmentInfoPopUp::OnExcludeButtonClicked()
{
	APokeCollectionCharacter* Player = Cast<APokeCollectionCharacter>(GetOwningPlayerPawn());
	if (!ensure(Player) || !GetOwningPlayer())
	{
		return;
	}

	APokeCollectionHUD* PokeHud = Cast<APokeCollectionHUD>(GetOwningPlayer()->GetHUD());
	if (!PokeHud)
	{
		return;
	}

	UInGameCharacterInfoWidget* InfoWidget = PokeHud->GetInGameCharacterInfoWidget();
	if (!InfoWidget)
	{
		return;
	}

	APokeCharacter* Character = Player->GetCharacterByID(InfoWidget->GetSelectedCharacterID());
	if (!ensure(Character))
	{
		return;
	}

	Character->TakeOffEquipment();

	InfoWidget->OnOpen();

	RemoveFromViewport();
}

void UEquipmentInfoPopUp::OnChangeButtonClicked()
{
	APokeCollectionHUD* PokeHud = GetOwningPlayer() ? Cast<APokeCollectionHUD>(GetOwningPlayer()->GetHUD()) : nullptr;
	if (!PokeHud)
	{
		return;
	}

	PokeHud->OpenInGameChangeEquipmentWidget();

	RemoveFromViewport();
}

void UEquipmentInfoPopUp::OnUpgradeButtonClicked()
{
	APokeCollectionHUD* PokeHud = GetOwningPlayer() ? Cast<APokeCollectionHUD>(GetOwningPlayer()->GetHUD()) : nullptr;
	if (!PokeHud)
	{
		return;
	}

	PokeHud->OpenEquipmentUpgradePopUp(EquipmentID);

	RemoveFromViewport();
}
