// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBoxWidget.h"

#include "UniformGridPanel.h"
#include "UniformGridSlot.h"
#include "ScrollBox.h"
#include "TextBlock.h"
#include "Image.h"
#include "Materials/MaterialInstanceDynamic.h"

#include "PokeCollectionCharacter.h"
#include "PokeCollectionHUD.h"
#include "PokeCharacter.h"

void UCharacterSlot::NativeConstruct()
{
	Super::NativeConstruct();

	BoxContentType = EBoxContentType::Character;
}

void UCharacterSlot::OnSelectButtonClicked()
{
	Super::OnSelectButtonClicked();

	APokeCollectionHUD* PokeHud = GetOwningPlayer() ? Cast<APokeCollectionHUD>(GetOwningPlayer()->GetHUD()) : nullptr;
	if (!PokeHud)
	{
		return;
	}
	
	PokeHud->OpenInGameCharacterInfoWidget(ContentID);
}

void UCharacterSlot::InitByID(int32 InContentID)
{
	Super::InitByID(InContentID);

	APokeCollectionCharacter* Player = Cast<APokeCollectionCharacter>(GetOwningPlayerPawn());
	if (!ensure(Player))
	{
		return;
	}

	APokeCharacter* Character = Player->GetCharacterByID(InContentID);
	if (!Character)
	{
		return;
	}

	SetContentLevel(Character->GetLevel());
	SetTypeImages(Character->GetCharacterType());
	SetJoinedPartyText(Character->GetJoinedPartyNum());
}

void UCharacterSlot::InitByKey(int32 InContentKey)
{
	Super::InitByKey(InContentKey);

	const FCharacterInfo* CharacterInfo = CMS::GetCharacterDataTable(InContentKey);
	if (!ensure(CharacterInfo))
	{
		return;
	}

	CharacterType CurrentType;
	CurrentType.Type1 = CharacterInfo->Type1;
	CurrentType.Type2 = CharacterInfo->Type2;

	SetTypeImages(CurrentType);
}

void UCharacterSlot::SetContentLevel(int32 InLevel)
{
	if (LevelText)
	{
		LevelText->SetText(FText::AsNumber(InLevel));
	}
}

void UCharacterSlot::SetTypeImages(CharacterType InCharacterType)
{
	TArray<UMaterialInstanceDynamic*> TypeIcons = PokeCore::GetTypeIcon(InCharacterType, this);

	if (Type1Image)
	{
		UMaterialInstanceDynamic* Type1Mat = TypeIcons[0];
		if (Type1Mat)
		{
			Type1Image->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Type1Image->SetBrushFromMaterial(Type1Mat);
		}
		else
		{
			Type1Image->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (Type2Image)
	{
		UMaterialInstanceDynamic* Type2Mat = TypeIcons[1];
		if (Type2Mat)
		{
			Type2Image->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Type2Image->SetBrushFromMaterial(Type2Mat);
		}
		else
		{
			Type2Image->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UCharacterSlot::SetJoinedPartyText(int32 InJoinedPartyNum)
{
	if (JoinedPartyText)
	{
		if (InJoinedPartyNum <= 0)
		{
			JoinedPartyText->SetVisibility(ESlateVisibility::Hidden);
			return;
		}

		JoinedPartyText->SetText(FText::FromString(FString::FromInt(InJoinedPartyNum)));
		JoinedPartyText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UCharacterBoxWidget::NativeConstruct() 
{
	BoxContentType = EBoxContentType::Character;

	Super::NativeConstruct();
}

void UCharacterBoxWidget::OnOpen()
{
	Super::OnOpen();

	SetFocus();

	RefreshSlot();
}

void UCharacterBoxWidget::RefreshSlot()
{
	Super::RefreshSlot();

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
		UCharacterSlot* BoxSlot = Cast<UCharacterSlot>(GridPanel->GetChildAt(Index));
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
