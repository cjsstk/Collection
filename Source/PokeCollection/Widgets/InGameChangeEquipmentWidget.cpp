// Copyright Me. All Rights Reserved.


#include "InGameChangeEquipmentWidget.h"

#include "Button.h"
#include "UniformGridPanel.h"
#include "UniformGridSlot.h"

#include "PokeCollectionCharacter.h"
#include "PokeCollectionHUD.h"
#include "PokeCharacter.h"
#include "PokeEquipment.h"
#include "Widgets/InGameCharacterInfoWidget.h"

void UChangeEquipmentSlot::NativeConstruct()
{
	Super::NativeConstruct();

	bUseOwnerCharacterImage = true;
}

void UChangeEquipmentSlot::InitByID(int32 InContentID)
{
	Super::InitByID(InContentID);

	SlotEquipmentID = InContentID;
}

void UChangeEquipmentSlot::OnSelectButtonClicked()
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

	Player->PutOnEquipment(InfoWidget->GetSelectedCharacterID(), SlotEquipmentID);

	PokeHud->OnBackButtonClicked(PokeHud->GetInGameChangeEquipmentWidget());
}

/** UExcludeEquipmentSlot */
void UExcludeEquipmentSlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (ExcludeButton)
	{
		ExcludeButton->OnClicked.AddUniqueDynamic(this, &UExcludeEquipmentSlot::OnExcludeButtonClicked);
	}
}

void UExcludeEquipmentSlot::OnExcludeButtonClicked()
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

	PokeHud->OnBackButtonClicked(PokeHud->GetInGameChangeEquipmentWidget());
}

/** UInGameChangeEquipmentWidget */
void UInGameChangeEquipmentWidget::NativeConstruct()
{
	Super::NativeConstruct();

	bIsFocusable = true;

	APokeCollectionCharacter* Player = GetPlayer();
	if (!ensure(Player))
	{
		return;
	}

	APokeCollectionHUD* PokeHud = GetPokeHud();
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
	if (!Character)
	{
		return;
	}

	if (!EquipmentGridPanel)
	{
		return;
	}

	EquipmentGridPanel->ClearChildren();

	NeedExcludeSlot = 0;

	UPokeEquipment* Equipment = Character->GetCurrentEquipment();
	if (Equipment)
	{
		NeedExcludeSlot = 1;

		UExcludeEquipmentSlot* ExcludeSlot = CreateWidget<UExcludeEquipmentSlot>(GetWorld(), ExcludeEquipmentSlotClass.Get());
		if (ensure(ExcludeSlot))
		{
			UUniformGridSlot* GridSlot = EquipmentGridPanel->AddChildToUniformGrid(ExcludeSlot, 0, 0);
			if (GridSlot)
			{
				GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
			}
		}
	}

	const int32 MaxHaveEquipmentsNum = Player->GetMaxHaveEquipmentNum() + NeedExcludeSlot;
	for (int32 Index = NeedExcludeSlot; Index < MaxHaveEquipmentsNum; ++Index)
	{
		UChangeEquipmentSlot* EquipmentSlot = CreateWidget<UChangeEquipmentSlot>(GetWorld(), EquipmentBoxSlotClass.Get());
		if (!ensure(EquipmentSlot))
		{
			return;
		}

		UUniformGridSlot* GridSlot = EquipmentGridPanel->AddChildToUniformGrid(EquipmentSlot, Index / ColumnNum, Index % ColumnNum);
		if (GridSlot)
		{
			GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
			//GridSlot->SetVerticalAlignment(VAlign_Fill);
		}
	}
}

void UInGameChangeEquipmentWidget::OnOpen()
{
	Super::OnOpen();

	SetFocus();

	APokeCollectionCharacter* Player = GetPlayer();
	if (!ensure(Player))
	{
		return;
	}

	if (!EquipmentGridPanel)
	{
		return;
	}

	if (!EquipmentBoxSlotClass.Get())
	{
		return;
	}

	const TArray<UPokeEquipment*>& HaveEquipments = Player->GetHaveEquipments();

	// Exclude equipment
	TArray<UPokeEquipment*> ShowEquipments = HaveEquipments;
	//ShowCharacters.RemoveAll([](const APokeCharacter* PC) { return PC->GetJoinedPartyNum() > 0; });

	int32 HasExcludeSlot = NeedExcludeSlot > 0 ? 1 : 0;
	const int32 SlotNum = EquipmentGridPanel->GetChildrenCount();

	for (int32 Index = 0; Index < SlotNum; Index++)
	{
		UChangeEquipmentSlot* EquipmentSlot = Cast<UChangeEquipmentSlot>(EquipmentGridPanel->GetChildAt(Index));
		if (EquipmentSlot)
		{
			if (ShowEquipments.IsValidIndex(Index - HasExcludeSlot))
			{
				UPokeEquipment* CurrentEquipment = ShowEquipments[Index - HasExcludeSlot];
				if (!ensure(CurrentEquipment))
				{
					continue;
				}

				EquipmentSlot->InitByID(CurrentEquipment->GetEquipmentID());
				//CharacterSlot->SetIsMakingParty(bIsMakingParty);
				EquipmentSlot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
			else
			{
				if ((Index / ColumnNum) > ((ShowEquipments.Num() - 1) / ColumnNum))
				{
					EquipmentSlot->SetVisibility(ESlateVisibility::Collapsed);
				}
				else
				{
					EquipmentSlot->SetVisibility(ESlateVisibility::Hidden);
				}
			}
		}
	}

}
