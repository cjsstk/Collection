// Fill out your copyright notice in the Description page of Project Settings.


#include "MakePartySlot.h"

#include "Button.h"

void UMakePartySlot::NativeConstruct()
{
	if (ChangeCharacterButton)
	{
		ChangeCharacterButton->OnClicked.AddUniqueDynamic(this, &UMakePartySlot::OnChangeCharacterButtonClicked);
	}
}

void UMakePartySlot::OnChangeCharacterButtonClicked()
{

}
