// Fill out your copyright notice in the Description page of Project Settings.


#include "MakePartySlot.h"

#include "Button.h"

#include "PokeCollectionHUD.h"
#include "Widgets/InGameMakePartyWidget.h"

void UMakePartySlot::NativeConstruct()
{
	if (ChangeCharacterButton)
	{
		ChangeCharacterButton->OnClicked.AddUniqueDynamic(this, &UMakePartySlot::OnChangeCharacterButtonClicked);
	}

	/*APokeCollectionHUD* PokeHud = GetOwningPlayer() ? Cast<APokeCollectionHUD>(GetOwningPlayer()->GetHUD()) : nullptr;
	if (ensure(PokeHud))
	{
		UInGameMakePartyWidget* MakePartyWidget = PokeHud->GetInGameMakePartyWidget();
		if (ensure(MakePartyWidget))
		{
			MakePartyWidget->AddCharacterSlot(this);
		}
	}*/
}

void UMakePartySlot::OnChangeCharacterButtonClicked()
{

}
