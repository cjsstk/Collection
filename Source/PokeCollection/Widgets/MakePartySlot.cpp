// Fill out your copyright notice in the Description page of Project Settings.


#include "MakePartySlot.h"

#include "Button.h"
#include "TextBlock.h"
#include "VerticalBox.h"

#include "PokeCharacter.h"
#include "PokeCollectionCharacter.h"
#include "PokeCollectionHUD.h"
#include "Widgets/CharacterBoxWidget.h"
#include "Widgets/InGameCharacterBoxWidget.h"
#include "Widgets/InGameMakePartyWidget.h"

void UMakePartySlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (SlotButton)
	{
		SlotButton->OnClicked.AddUniqueDynamic(this, &UMakePartySlot::OnSlotButtonClicked);
	}
}

void UMakePartySlot::RefreshSlot()
{
	if (!SlotImage)
	{
		return;
	}

	if (!SlotCharacterInfoBox)
	{
		return;
	}

	if (!SlotCharacter)
	{
		return;
	}

	APokeCollectionCharacter* Player = Cast<APokeCollectionCharacter>(GetOwningPlayerPawn());
	if (!ensure(Player))
	{
		return;
	}

	int32 CurrentPartyNum = Player->GetCurrentSelectedPartyNum();

	const TMap<int32, APokeCharacter*> PartyCharacters = Player->GetPartyCharacters(CurrentPartyNum);

	if (PartyCharacters.Contains(SlotNum))
	{
		APokeCharacter* CurrentSlotCharacter = PartyCharacters[SlotNum];
		if (ensure(CurrentSlotCharacter))
		{
			SlotCharacterInfoBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			SlotCharacter->InitByID(CurrentSlotCharacter->GetCharacterID());

			const FStatus& CharacterStatus = CurrentSlotCharacter->GetFinalStatus();

			SlotCharacterHealth->SetText(FText::AsCultureInvariant(FString::FromInt(CharacterStatus.HealthPoint)));
			SlotCharacterAttack->SetText(FText::AsCultureInvariant(FString::FromInt(CharacterStatus.Attack)));
			SlotCharacterDefense->SetText(FText::AsCultureInvariant(FString::FromInt(CharacterStatus.Defense)));
			SlotCharacterSpAttack->SetText(FText::AsCultureInvariant(FString::FromInt(CharacterStatus.SpecialAttack)));
			SlotCharacterSpDefense->SetText(FText::AsCultureInvariant(FString::FromInt(CharacterStatus.SpecialDefense)));
			SlotCharacterSpeed->SetText(FText::AsCultureInvariant(FString::FromInt(CharacterStatus.Speed)));
			SlotCharacterConsume->SetText(FText::AsCultureInvariant(FString::FromInt(CurrentSlotCharacter->GetConsumeBerryAmount())));

			bIsEmptySlot = false;
		}
	}
	else
	{
		SlotCharacterInfoBox->SetVisibility(ESlateVisibility::Collapsed);
		bIsEmptySlot = true;
	}
	
}

void UMakePartySlot::OnSlotButtonClicked()
{
	APokeCollectionHUD* PokeHud = Cast<APokeCollectionHUD>(GetOwningPlayer()->GetHUD());
	if (PokeHud)
	{
		if (PokeHud->GetInGameCharacterBoxWidget()->IsInViewport())
		{
			return;
		}

		PokeHud->OpenInGameCharacterBoxWidget(bIsEmptySlot);
		UInGameCharacterBoxWidget* CharacterBoxWidget = PokeHud->GetInGameCharacterBoxWidget();
		if (CharacterBoxWidget)
		{
			CharacterBoxWidget->SetSelectedPartySlotNum(SlotNum);
		}
	}
}
