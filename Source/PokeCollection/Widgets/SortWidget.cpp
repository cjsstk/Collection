// Fill out your copyright notice in the Description page of Project Settings.


#include "SortWidget.h"

#include "Image.h"
#include "Button.h"
#include "Components/CanvasPanelSlot.h"
#include "UniformGridPanel.h"
#include "UniformGridSlot.h"
#include "TextBlock.h"

#include "PokeCollectionHUD.h"
#include "Widgets/InGameBoxWidget.h"

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

	for (int32 KindIndex = 0; KindIndex < SortInfos.Num(); ++KindIndex)
	{
		USortPopUpButton* SortPopUpButton = CreateWidget<USortPopUpButton>(GetWorld(), SortPopUpButtonClass);
		if (!SortPopUpButton)
		{
			ensure(0);
			continue;
		}

		//
		ESortCategory CurrentCategory = SortInfos[KindIndex].SortCategory;
		ESortAscending CurrentAscending = SortInfos[KindIndex].SortAscending;

		const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ESortCategory"), true);
		if (!ensure(enumPtr))
		{
			return;
		}

		const UEnum* AenumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ESortAscending"), true);
		if (!ensure(AenumPtr))
		{
			return;
		}

		FString CurrentCategoryString = enumPtr->GetNameStringByIndex((int32)CurrentCategory);
		FString CurrentAscendingString = AenumPtr->GetNameStringByIndex((int32)CurrentAscending);

		SortPopUpButton->SetSortName(FText::FromString(CurrentCategoryString + CurrentAscendingString));
		//

		SortPopUpButton->SetSortInfo(SortInfos[KindIndex]);

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

		APokeCollectionHUD* PokeHud = Cast<APokeCollectionHUD>(GetOwningPlayer()->GetHUD());
		if (PokeHud)
		{
			UInGameBoxWidget* BoxWidget = PokeHud->GetInGameBoxWidget();
			if (BoxWidget)
			{
				OnSortPopUpButtonClicked.AddUniqueDynamic(BoxWidget, &UInGameBoxWidget::SortContentWidget);
			}
		}
	}
}

void USortPopUpButton::OnSelectButtonClicked()
{
	OnSortPopUpButtonClicked.Broadcast(SortInfo);
}

void USortPopUpButton::SetSortName(FText InSortName)
{
	if (SortNameText)
	{
		SortNameText->SetText(InSortName);
	}
}

void USortPopUpButton::SetSortInfo(FPokeSortInfo InSortInfo)
{
	SortInfo = InSortInfo;
}