// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameBoxWidget.h"

#include "Button.h"
#include "TextBlock.h"
#include "Image.h"
#include "WidgetSwitcher.h"
#include "UniformGridPanel.h"
#include "UniformGridSlot.h"
#include "ScrollBox.h"
#include "LogMacros.h"

#include "PokeCharacter.h"
#include "PokeCollectionCharacter.h"
#include "PokeCollectionHUD.h"
#include "PokeEquipment.h"
#include "Widgets/SortWidget.h"

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

//FReply UBoxContentWidget::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
//{
//	FVector2D SP = InGestureEvent.GetScreenSpacePosition();
//	FVector2D LSP = InGestureEvent.GetLastScreenSpacePosition();
//
//	InGeometry.AbsoluteToLocal(SP);
//
//	UE_LOG(LogTemp, Log, TEXT("SP (%.1f, %.1f) / LSP (%.1f, %.1f)"), SP.X, SP.Y, LSP.X, LSP.Y);
//
//	return Super::NativeOnTouchMoved(InGeometry, InGestureEvent);
//}

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

	ContentsBox->SetActiveWidgetIndex((int32)CurrentBoxContentType);
	EquipmentSortWidget->SetVisibility(ESlateVisibility::Collapsed);
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
	CurrentBoxContentType = EBoxContentType::Character;

	ContentsBox->SetActiveWidgetIndex((int32)CurrentBoxContentType);
	BoxContents[0]->OnOpen();

	CharacterSortWidget->SetVisibility(ESlateVisibility::Visible);
	EquipmentSortWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void UInGameBoxWidget::OnEquipmentBoxButtonClicked()
{
	CurrentBoxContentType = EBoxContentType::Equipment;

	ContentsBox->SetActiveWidgetIndex((int32)CurrentBoxContentType);
	BoxContents[1]->OnOpen();

	CharacterSortWidget->SetVisibility(ESlateVisibility::Collapsed);
	EquipmentSortWidget->SetVisibility(ESlateVisibility::Visible);
}

void UBoxSlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (SelectContentButton)
	{
		SelectContentButton->SetTouchMethod(EButtonTouchMethod::PreciseTap);
		SelectContentButton->OnClicked.AddUniqueDynamic(this, &UBoxSlot::OnSelectContentButtonClicked);
	}
}

void UBoxSlot::InitByKey(int32 InContentKey)
{
	CharacterKey = InContentKey;

	switch (BoxContentType)
	{
	case EBoxContentType::Character:
	{
		const FCharacterInfo* CharacterInfo = CMS::GetCharacterDataTable(InContentKey);
		if (ensure(CharacterInfo))
		{
			SetContentImage(CharacterInfo->CharacterProfile);
			SetContentName(FText::FromName(CharacterInfo->CharacterName));
			SetBackgroundColor(CharacterInfo->CharacterRank);
		}
		break;
	}
	case EBoxContentType::Equipment:
	{
		const FEquipmentInfo* EquipmentInfo = CMS::GetEquipmentDataTable(InContentKey);
		if (ensure(EquipmentInfo))
		{
			SetContentImage(EquipmentInfo->EquipmentProfile);
			SetContentName(FText::FromName(EquipmentInfo->EquipmentName));
			SetBackgroundColor(EquipmentInfo->EquipmentRank);
		}
		break;
	}
	default:
		break;
	}
}

void UBoxSlot::InitByID(int32 InContentID)
{
	SetContentID(InContentID);

	APokeCollectionCharacter* Player = Cast<APokeCollectionCharacter>(GetOwningPlayerPawn());
	if (!ensure(Player))
	{
		return;
	}

	switch (BoxContentType)
	{
	case EBoxContentType::Character:
	{
		APokeCharacter* Character = Player->GetCharacterByID(InContentID);
		if (!Character)
		{
			return;
		}

		int32 CurrentCharacterKey = Character->GetCharacterKey();
		InitByKey(CurrentCharacterKey);
		break;
	}
	case EBoxContentType::Equipment:
	{
		UPokeEquipment* Equipment = Player->GetEquipmentByID(InContentID);
		if (!Equipment)
		{
			return;
		}

		int32 EquipmentKey = Equipment->GetEquipmentKey();
		InitByKey(EquipmentKey);
		break;
	}
	default:
		break;
	}
}

void UBoxSlot::OnSelectContentButtonClicked()
{
	OnSelectButtonClicked();
}

void UBoxSlot::SetContentImage(UTexture2D* InContentTexture)
{
	if (!InContentTexture)
	{
		return;
	}

	ProfileImage->SetBrushFromTexture(InContentTexture);
}

void UBoxSlot::SetContentID(int32 InContentID)
{
	ContentID = InContentID;
}

void UBoxSlot::SetContentName(const FText& InName)
{
	if (NameText)
	{
		NameText->SetText(InName);
	}
}

void UBoxSlot::SetBackgroundColor(ERank InRank)
{
	if (BackgroundImage)
	{
		FLinearColor Color;

		switch (InRank)
		{
		case ERank::Normal:
			Color = NormalColor;
			break;
		case ERank::Rare:
			Color = RareColor;
			break;
		case ERank::SRare:
			Color = SRareColor;
			break;
		case ERank::SSRare:
			Color = SSRareColor;
			break;
		default:
			break;
		}

		BackgroundImage->SetColorAndOpacity(Color);
	}
}
