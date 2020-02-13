// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameTopStatusBar.h"

#include "Button.h"

void UInGameTopStatusBar::NativeConstruct()
{
	if (BackButton)
	{
		BackButton->OnClicked.AddUniqueDynamic(this, &UInGameTopStatusBar::OnBack);
	}

}

void UInGameTopStatusBar::OnBack()
{
	OnBackButtonClicked.Broadcast();
}
