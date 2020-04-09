// Fill out your copyright notice in the Description page of Project Settings.


#include "SortWidget.h"

#include "Image.h"
#include "Components/CanvasPanelSlot.h"
#include "UniformGridPanel.h"
#include "UniformGridSlot.h"
#include "TextBlock.h"

void USortWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (SortButton)
	{
		SortButton->OnClicked.AddUniqueDynamic(this, &USortWidget::OnSortButtonClicked);
	}
}

void USortWidget::OnSortButtonClicked()
{
	if (SortMenuPopUp)
	{
		SortMenuPopUp->AddToViewport(1);
		return;
	}

	if (ensure(SortMenuPopUpClass.Get()))
	{
		SortMenuPopUp = CreateWidget<USortMenuPopUp>(GetWorld(), SortMenuPopUpClass);
		if (!SortMenuPopUp)
		{
			ensure(0);
			return;
		}
		
		SortMenuPopUp->AddToViewport(1);
	}
}

void USortMenuPopUp::NativeConstruct()
{
	Super::NativeConstruct();

	if (BackgroundButton)
	{
		BackgroundButton->OnClicked.AddUniqueDynamic(this, &USortMenuPopUp::OnBackgroundClicked);
	}

	if (!ensure(SortPopUpButtonClass.Get()))
	{
		return;
	}

	ButtonGridPanel->ClearChildren();

	for (int32 KindIndex = 0; KindIndex < SortKinds.Num(); ++KindIndex)
	{
		USortPopUpButton* SortPopUpButton = CreateWidget<USortPopUpButton>(GetWorld(), SortPopUpButtonClass);
		if (!SortPopUpButton)
		{
			ensure(0);
			continue;
		}

		ESortKinds CurrentSort = SortKinds[KindIndex];

		const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ESortKinds"), true);
		if (!ensure(enumPtr))
		{
			return;
		}

		FString SortNameString = enumPtr->GetNameStringByIndex((int32)CurrentSort);

		SortPopUpButton->SetSortName(FText::FromString(SortNameString));

		UUniformGridSlot* GridSlot = ButtonGridPanel->AddChildToUniformGrid(SortPopUpButton, KindIndex / ColumnNum, KindIndex % ColumnNum);
		if (ensure(GridSlot))
		{
			GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
			GridSlot->SetVerticalAlignment(VAlign_Fill);
		}
	}

}

void USortMenuPopUp::OnBackgroundClicked()
{
	RemoveFromViewport();
}

void USortPopUpButton::NativeConstruct()
{
	Super::NativeConstruct();

	if (SortSelectButton)
	{
		SortSelectButton->OnClicked.AddUniqueDynamic(this, &USortPopUpButton::OnSelectButtonClicked);
	}
}

void USortPopUpButton::OnSelectButtonClicked()
{

}

void USortPopUpButton::SetSortName(FText InSortName)
{
	if (SortNameText)
	{
		SortNameText->SetText(InSortName);
	}
}
