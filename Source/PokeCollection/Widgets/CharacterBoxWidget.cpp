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

	if (TypeMaterial)
	{
		Type1MaterialInstance = UMaterialInstanceDynamic::Create(TypeMaterial, this);
		Type2MaterialInstance = UMaterialInstanceDynamic::Create(TypeMaterial, this);
	}
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
	if (!Type1MaterialInstance || !Type2MaterialInstance || !Type1Image || !Type2Image)
	{
		ensure(0);
		return;
	}

	const FTypeInfo* Type1Info = CMS::GetTypeDataTable(InCharacterType.Type1);
	const FTypeInfo* Type2Info = CMS::GetTypeDataTable(InCharacterType.Type2);

	int32 Type1TextureNum = Type1Info ? Type1Info->TextureNum : -1;
	int32 Type2TextureNum = Type2Info ? Type2Info->TextureNum : -1;

	if (Type1TextureNum < 0)
	{
		Type1Image->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		Type1Image->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		int32 Type1ColumnIndex = Type1TextureNum % CharacterTypeColumnNum;
		int32 Type1RawIndex = Type1TextureNum / CharacterTypeColumnNum;

		Type1MaterialInstance->SetScalarParameterValue("ColumnIndexParam", Type1ColumnIndex);
		Type1MaterialInstance->SetScalarParameterValue("RowIndexParam", Type1RawIndex);

		Type1Image->SetBrushFromMaterial(Type1MaterialInstance);
	}

	if (Type2TextureNum < 0)
	{
		Type2Image->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		Type2Image->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		int32 Type2ColumnIndex = Type2TextureNum % CharacterTypeColumnNum;
		int32 Type2RawIndex = Type2TextureNum / CharacterTypeColumnNum;

		Type2MaterialInstance->SetScalarParameterValue("ColumnIndexParam", Type2ColumnIndex);
		Type2MaterialInstance->SetScalarParameterValue("RowIndexParam", Type2RawIndex);

		Type2Image->SetBrushFromMaterial(Type2MaterialInstance);
	}

}

void UCharacterBoxWidget::NativeConstruct() 
{
	Super::NativeConstruct();
}

void UCharacterBoxWidget::OnOpen()
{
	Super::OnOpen();

	SetFocus();

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

	const int32 SlotNum = GridPanel->GetChildrenCount();

	for (int32 Index = 0; Index < SlotNum; Index++)
	{
		UCharacterSlot* BoxSlot = Cast<UCharacterSlot>(GridPanel->GetChildAt(Index));
		if (BoxSlot)
		{
			if (HaveCharacters.IsValidIndex(Index))
			{
				APokeCharacter* CurrentCharacter = HaveCharacters[Index];
				if (!ensure(CurrentCharacter))
				{
					continue;
				}

				BoxSlot->InitByID(CurrentCharacter->GetCharacterID());
				BoxSlot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
			else
			{
				if ((Index / ColumnNum) > ((HaveCharacters.Num() - 1) / ColumnNum))
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
