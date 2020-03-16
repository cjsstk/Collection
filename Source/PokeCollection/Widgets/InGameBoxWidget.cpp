// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameBoxWidget.h"

#include "Button.h"
#include "Image.h"
#include "WidgetSwitcher.h"
#include "UniformGridPanel.h"
#include "UniformGridSlot.h"
#include "ScrollBox.h"

#include "PokeCollectionCharacter.h"

void UBoxContentWidget::NativeConstruct()
{
	Super::NativeConstruct();

	bIsFocusable = true;

	APokeCollectionCharacter* Player = Cast<APokeCollectionCharacter>(GetOwningPlayerPawn());
	if (!ensure(Player))
	{
		return;
	}

	if (!GridPanel)
	{
		return;
	}

	GridPanel->ClearChildren();

	int32 MaxHaveNum = 0;
	switch (BoxContentType)
	{
	case EBoxContentType::Character:
		MaxHaveNum = Player->GetMaxHaveCharactersNum();
		break;
	case EBoxContentType::Equipment:
		MaxHaveNum = Player->GetMaxHaveCharactersNum();
		break;
	default:
		break;
	}

	for (int32 Index = 0; Index < MaxHaveNum; ++Index)
	{
		UBoxSlot* BoxSlot = CreateWidget<UBoxSlot>(GetWorld(), BoxSlotClass.Get());
		if (!ensure(BoxSlot))
		{
			return;
		}

		UUniformGridSlot* GridSlot = GridPanel->AddChildToUniformGrid(BoxSlot, Index / ColumnNum, Index % ColumnNum);
		if (GridSlot)
		{
			GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
			//GridSlot->SetVerticalAlignment(VAlign_Fill);
		}
	}

}

void UInGameBoxWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (CharacterBoxButton)
	{
		CharacterBoxButton->OnClicked.AddUniqueDynamic(this, &UInGameBoxWidget::OnCharacterBoxButtonClicked);
	}

	if (EquipmentBoxButton)
	{
		EquipmentBoxButton->OnClicked.AddUniqueDynamic(this, &UInGameBoxWidget::OnEquipmentBoxButtonClicked);
	}

	ContentsBox->ClearChildren();
	BoxContents.Empty();

	for (auto&& ContentWidget : ContentWidgets)
	{
		UBoxContentWidget* BoxContentWidget = CreateWidget<UBoxContentWidget>(GetOwningPlayer(), ContentWidget.ContentWidgetClass);
		if (!BoxContentWidget)
		{
			return;
		}

		ContentsBox->AddChild(BoxContentWidget);
		BoxContents.AddUnique(BoxContentWidget);
	}

	ContentsBox->SetActiveWidgetIndex(0);
}

void UInGameBoxWidget::OnOpen()
{
	for (auto&& BoxContent : BoxContents)
	{
		if (!BoxContent)
		{
			return;
		}

		BoxContent->OnOpen();
	}
}

void UInGameBoxWidget::OnCharacterBoxButtonClicked()
{
	ContentsBox->SetActiveWidgetIndex(0);
	BoxContents[0]->OnOpen();
}

void UInGameBoxWidget::OnEquipmentBoxButtonClicked()
{
	ContentsBox->SetActiveWidgetIndex(1);
	BoxContents[1]->OnOpen();
}

void UBoxSlot::SetContentImage(UTexture2D* InContentTexture)
{
	if (!InContentTexture)
	{
		return;
	}

	ProfileImage->SetBrushFromTexture(InContentTexture);
}

void UBoxSlot::SetContentID(EBoxContentType InBoxContentType, int32 InContentID)
{
	BoxContentType = InBoxContentType;
	ContentID = InContentID;
}
