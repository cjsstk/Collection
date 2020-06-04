// Copyright Me. All Rights Reserved.


#include "BasicPopUp.h"

#include "Button.h"
#include "TextBlock.h"

void UBasicPopUp::NativeConstruct()
{
	Super::NativeConstruct();

	if (ConfirmButton)
	{
		ConfirmButton->OnClicked.AddUniqueDynamic(this, &UBasicPopUp::OnConfirmButtonClicked);
	}

	if (BackgroundButton)
	{
		BackgroundButton->OnClicked.AddUniqueDynamic(this, &UBasicPopUp::OnBackgroundClicked);
	}
}

void UBasicPopUp::SetConfirmText(FText InText)
{
	if (ConfirmText)
	{
		ConfirmText->SetText(InText);
	}
}

void UBasicPopUp::OnBackgroundClicked()
{
	RemoveFromViewport();
}

void UBasicPopUp::OnConfirmButtonClicked()
{
	OnConfirmed();

	RemoveFromViewport();
}
