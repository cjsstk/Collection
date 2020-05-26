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
#include "PokeItem.h"
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
	case EBoxContentType::Item:
		MaxHaveNum = CMS::GetItemDataNum();
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

void UBoxContentWidget::SortContent(FPokeSortInfo InSortInfo)
{
	CurrentSortInfo = InSortInfo;

	RefreshSlot();
}

TArray<class ISortObjectInterface*> UBoxContentWidget::SortObject(TArray<class ISortObjectInterface*> InObjects)
{
	QuickSort(0, InObjects.Num() - 1, InObjects);

	return InObjects;
}

void UBoxContentWidget::QuickSort(int32 Start, int32 End, TArray<class ISortObjectInterface*>& InObjects)
{
	if (Start >= End)
	{
		return;
	}

	ISortObjectInterface* Pivot = InObjects[(Start + End) / 2];
	int Left = Start;
	int Right = End;

	bool bAscending = CurrentSortInfo.SortAscending == ESortAscending::Ascending;

	while (Left <= Right)
	{
		if (bAscending)
		{
			while (InObjects[Left]->GetObjectSortValue(CurrentSortInfo.SortCategory) < Pivot->GetObjectSortValue(CurrentSortInfo.SortCategory))
			{
				Left++;
			}

			while (InObjects[Right]->GetObjectSortValue(CurrentSortInfo.SortCategory) > Pivot->GetObjectSortValue(CurrentSortInfo.SortCategory))
			{
				Right--;
			}
		}
		else
		{
			while (InObjects[Left]->GetObjectSortValue(CurrentSortInfo.SortCategory) > Pivot->GetObjectSortValue(CurrentSortInfo.SortCategory))
			{
				Left++;
			}

			while (InObjects[Right]->GetObjectSortValue(CurrentSortInfo.SortCategory) < Pivot->GetObjectSortValue(CurrentSortInfo.SortCategory))
			{
				Right--;
			}
		}

		if (Left <= Right)
		{
			InObjects.Swap(Left, Right);
			Left++;
			Right--;
		}
	}

	QuickSort(Start, Right, InObjects);
	QuickSort(Left, End, InObjects);
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

	if (ItemBoxButton)
	{
		ItemBoxButton->OnClicked.AddUniqueDynamic(this, &UInGameBoxWidget::OnItemBoxButtonClicked);
	}

	ContentsBox->ClearChildren();
	BoxContents.Empty();
	BoxContents.Init(nullptr, ContentWidgets.Num());

	for (auto&& ContentWidget : ContentWidgets)
	{
		UBoxContentWidget* BoxContentWidget = CreateWidget<UBoxContentWidget>(GetOwningPlayer(), ContentWidget.ContentWidgetClass);
		if (!BoxContentWidget)
		{
			return;
		}

		ContentsBox->AddChild(BoxContentWidget);
		BoxContents[(int32)ContentWidget.BoxContentType] = BoxContentWidget;
	}

	ContentsBox->SetActiveWidgetIndex((int32)CurrentBoxContentType);
	EquipmentSortWidget->SetVisibility(ESlateVisibility::Collapsed);
	ItemSortWidget->SetVisibility(ESlateVisibility::Collapsed);
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

	if (!BoxContents[(int32)CurrentBoxContentType])
	{
		ensure(0);
		return;
	}

	ContentsBox->SetActiveWidgetIndex((int32)CurrentBoxContentType);
	BoxContents[(int32)CurrentBoxContentType]->OnOpen();

	CharacterSortWidget->SetVisibility(ESlateVisibility::Visible);
	EquipmentSortWidget->SetVisibility(ESlateVisibility::Collapsed);
	ItemSortWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void UInGameBoxWidget::OnEquipmentBoxButtonClicked()
{
	CurrentBoxContentType = EBoxContentType::Equipment;

	if (!BoxContents[(int32)CurrentBoxContentType])
	{
		ensure(0);
		return;
	}

	ContentsBox->SetActiveWidgetIndex((int32)CurrentBoxContentType);
	BoxContents[(int32)CurrentBoxContentType]->OnOpen();

	CharacterSortWidget->SetVisibility(ESlateVisibility::Collapsed);
	EquipmentSortWidget->SetVisibility(ESlateVisibility::Visible);
	ItemSortWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void UInGameBoxWidget::OnItemBoxButtonClicked()
{
	CurrentBoxContentType = EBoxContentType::Item;

	if (!BoxContents[(int32)CurrentBoxContentType])
	{
		ensure(0);
		return;
	}

	ContentsBox->SetActiveWidgetIndex((int32)CurrentBoxContentType);
	BoxContents[(int32)CurrentBoxContentType]->OnOpen();

	CharacterSortWidget->SetVisibility(ESlateVisibility::Collapsed);
	EquipmentSortWidget->SetVisibility(ESlateVisibility::Collapsed);
	ItemSortWidget->SetVisibility(ESlateVisibility::Visible);
}

void UInGameBoxWidget::SortContentWidget(FPokeSortInfo InSortInfo)
{
	UE_LOG(LogTemp, Log, TEXT("SortButtonClicked"));

	if (!BoxContents[(int32)CurrentBoxContentType])
	{
		ensure(0);
		return;
	}

	BoxContents[(int32)CurrentBoxContentType]->SortContent(InSortInfo);
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
	ContentKey = InContentKey;

	switch (BoxContentType)
	{
	case EBoxContentType::Character:
	{
		const FCharacterInfo* CharacterInfo = CMS::GetCharacterDataTable(InContentKey);
		if (ensure(CharacterInfo))
		{
			SetContentImage(CharacterInfo->CharacterProfile.LoadSynchronous());
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
			SetContentImage(EquipmentInfo->EquipmentProfile.LoadSynchronous());
			SetContentName(FText::FromName(EquipmentInfo->EquipmentName));
			SetBackgroundColor(EquipmentInfo->EquipmentRank);
		}
		break;
	}
	case EBoxContentType::Item:
	{
		const FPokeItemInfo* ItemInfo = CMS::GetItemDataTable(InContentKey);
		if (ensure(ItemInfo))
		{
			SetContentImage(ItemInfo->ItemIcon.LoadSynchronous());
			SetContentName(FText::FromName(ItemInfo->ItemName));
			SetBackgroundColor(ItemInfo->ItemRank);
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
	case EBoxContentType::Item:
	{
		UPokeItem* Item = Player->GetItemByID(InContentID);
		if (!Item)
		{
			return;
		}

		int32 ItemKey = Item->GetItemKey();
		InitByKey(ItemKey);
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
