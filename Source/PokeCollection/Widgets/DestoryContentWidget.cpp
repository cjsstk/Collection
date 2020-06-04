// Copyright Me. All Rights Reserved.


#include "DestoryContentWidget.h"

#include "UniformGridPanel.h"
#include "Button.h"
#include "Image.h"

#include "PokeCharacter.h"
#include "PokeCollectionCharacter.h"
#include "PokeCollectionHUD.h"
#include "PokeEquipment.h"

void UDestoryCharacterSlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (SelectedImage)
	{
		SelectedImage->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UDestoryCharacterSlot::OnSelectButtonClicked()
{
	bSelected = !bSelected;

	if (bSelected)
	{
		if (SelectedImage)
		{
			SelectedImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
	else
	{
		if (SelectedImage)
		{
			SelectedImage->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	OnDestroySlotClicked.Broadcast(bSelected, ContentID);
}

void UDestoryCharacterSlot::InitByKey(int32 InContentKey)
{
	Super::InitByKey(InContentKey);

	if (SelectedImage)
	{
		SelectedImage->SetVisibility(ESlateVisibility::Collapsed);
	}
}

////
void UDestoryEquipmentSlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (SelectedImage)
	{
		SelectedImage->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UDestoryEquipmentSlot::OnSelectButtonClicked()
{
	bSelected = !bSelected;

	if (bSelected)
	{
		if (SelectedImage)
		{
			SelectedImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
	else
	{
		if (SelectedImage)
		{
			SelectedImage->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	OnDestroySlotClicked.Broadcast(bSelected, ContentID);
}


void UDestoryEquipmentSlot::InitByKey(int32 InContentKey)
{
	Super::InitByKey(InContentKey);

	if (SelectedImage)
	{
		SelectedImage->SetVisibility(ESlateVisibility::Collapsed);
	}
}

////
void UDestoryContentWidget::NativeConstruct()
{
	BoxContentType = EBoxContentType::Character;

	Super::NativeConstruct();

	if (CharacterButton)
	{
		CharacterButton->OnClicked.AddUniqueDynamic(this, &UDestoryContentWidget::OnCharacterButtonClicked);
	}

	if (EquipmentButton)
	{
		EquipmentButton->OnClicked.AddUniqueDynamic(this, &UDestoryContentWidget::OnEquipmentButtonClicked);
	}

	if (DestroyButton)
	{
		DestroyButton->OnClicked.AddUniqueDynamic(this, &UDestoryContentWidget::OnDestroyButtonClicked);
	}
}

void UDestoryContentWidget::OnOpen()
{
	Super::OnOpen();

	SetFocus();

	RefreshSlot();
}

void UDestoryContentWidget::RefreshSlot()
{
	Super::RefreshSlot();

	switch (BoxContentType)
	{
	case EBoxContentType::Character:
		RefreshCharacter();
		break;
	case EBoxContentType::Equipment:
		RefreshEquipment();
		break;
	default:
		break;
	}
}

void UDestoryContentWidget::OnCharacterButtonClicked()
{
	BoxContentType = EBoxContentType::Character;
	BoxSlotClass = CharacterSlotClass;
	ColumnNum = CharacterSlotColumnNum;

	InitSlot();
	RefreshSlot();
}

void UDestoryContentWidget::OnEquipmentButtonClicked()
{
	BoxContentType = EBoxContentType::Equipment;
	BoxSlotClass = EquipmentSlotClass;
	ColumnNum = EquipmentSlotColumnNum;

	InitSlot();
	RefreshSlot();
}

void UDestoryContentWidget::OnDestroyButtonClicked()
{
	APokeCollectionHUD* PokeHud = Cast<APokeCollectionHUD>(GetOwningPlayer()->GetHUD());
	if (!PokeHud)
	{
		return;
	}

	if (BoxContentType == EBoxContentType::Character)
	{
		PokeHud->OpenBasicPopUp(EBasicPopUpType::DestroyCharacter);
	}
	else
	{
		PokeHud->OpenBasicPopUp(EBasicPopUpType::DestroyEquipment);
	}
}

void UDestoryContentWidget::OnDestroySlotSelected(bool bSelected, int32 InContentID)
{
	APokeCollectionCharacter* Player = Cast<APokeCollectionCharacter>(GetOwningPlayerPawn());
	if (!ensure(Player))
	{
		return;
	}

	if (bSelected)
	{
		if (BoxContentType == EBoxContentType::Character)
		{
			Player->SeletedDestroyCharacterIDs.AddUnique(InContentID);
		}
		else
		{
			Player->SeletedDestroyEquipmentIDs.AddUnique(InContentID);
		}
	}
	else
	{
		if (BoxContentType == EBoxContentType::Character)
		{
			Player->SeletedDestroyCharacterIDs.Remove(InContentID);
		}
		else
		{
			Player->SeletedDestroyEquipmentIDs.Remove(InContentID);
		}
	}
}

void UDestoryContentWidget::RefreshCharacter()
{
	APokeCollectionCharacter* Player = Cast<APokeCollectionCharacter>(GetOwningPlayerPawn());
	if (!ensure(Player))
	{
		return;
	}

	if (!GridPanel)
	{
		return;
	}

	if (!BoxSlotClass.Get())
	{
		return;
	}

	Player->SeletedDestroyCharacterIDs.Empty();

	const TArray<APokeCharacter*>& HaveCharacters = Player->GetHaveCharacters();
	//

	TArray<ISortObjectInterface*> HaveSortCharacters;

	for (auto&& HaveCharacter : HaveCharacters)
	{
		HaveSortCharacters.Add(HaveCharacter);
	}

	HaveSortCharacters = SortObject(HaveSortCharacters);

	//
	const int32 SlotNum = GridPanel->GetChildrenCount();

	for (int32 Index = 0; Index < SlotNum; Index++)
	{
		UDestoryCharacterSlot* BoxSlot = Cast<UDestoryCharacterSlot>(GridPanel->GetChildAt(Index));
		if (BoxSlot)
		{
			if (HaveSortCharacters.IsValidIndex(Index))
			{
				APokeCharacter* CurrentCharacter = Cast<APokeCharacter>(HaveSortCharacters[Index]);
				if (!ensure(CurrentCharacter))
				{
					continue;
				}

				BoxSlot->InitByID(CurrentCharacter->GetCharacterID());
				BoxSlot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				BoxSlot->OnDestroySlotClicked.AddUniqueDynamic(this, &UDestoryContentWidget::OnDestroySlotSelected);
			}
			else
			{
				if ((Index / ColumnNum) > ((HaveSortCharacters.Num() - 1) / ColumnNum))
				{
					BoxSlot->SetVisibility(ESlateVisibility::Collapsed);
				}
				else
				{
					BoxSlot->SetVisibility(ESlateVisibility::Hidden);
				}
			}
		}

	}
}

void UDestoryContentWidget::RefreshEquipment()
{
	APokeCollectionCharacter* Player = Cast<APokeCollectionCharacter>(GetOwningPlayerPawn());
	if (!ensure(Player))
	{
		return;
	}

	if (!GridPanel)
	{
		return;
	}

	if (!BoxSlotClass.Get())
	{
		return;
	}

	Player->SeletedDestroyEquipmentIDs.Empty();

	const TArray<UPokeEquipment*>& HaveEquipments = Player->GetHaveEquipments();

	const int32 SlotNum = GridPanel->GetChildrenCount();

	for (int32 Index = 0; Index < SlotNum; Index++)
	{
		UDestoryEquipmentSlot* BoxSlot = Cast<UDestoryEquipmentSlot>(GridPanel->GetChildAt(Index));
		if (BoxSlot)
		{
			if (HaveEquipments.IsValidIndex(Index))
			{
				UPokeEquipment* CurrentEquipment = HaveEquipments[Index];
				if (!ensure(CurrentEquipment))
				{
					continue;
				}

				BoxSlot->InitByID(CurrentEquipment->GetEquipmentID());
				BoxSlot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				BoxSlot->OnDestroySlotClicked.AddUniqueDynamic(this, &UDestoryContentWidget::OnDestroySlotSelected);
			}
			else
			{
				if ((Index / ColumnNum) > ((HaveEquipments.Num() - 1) / ColumnNum))
				{
					BoxSlot->SetVisibility(ESlateVisibility::Collapsed);
				}
				else
				{
					BoxSlot->SetVisibility(ESlateVisibility::Hidden);
				}
			}
		}

	}
}
