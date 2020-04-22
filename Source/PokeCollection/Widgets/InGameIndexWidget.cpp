// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameIndexWidget.h"

#include "Button.h"
#include "UniformGridPanel.h"
#include "UniformGridSlot.h"
#include "ScrollBox.h"
#include "Image.h"
#include "TextBlock.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "VerticalBox.h"

#include "CMS.h"
#include "PokeCharacter.h"
#include "PokeCollectionCharacter.h"

void UInGameIndexWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetFocus();

	if (!CharacterGridPanel)
	{
		return;
	}

	if (!CharacterIndexSlotClass.Get())
	{
		return;
	}

	CharacterGridPanel->ClearChildren();

	TArray<FCharacterInfo*> AllCharacters;
	CMS::GetAllCharacterDataTable(AllCharacters);

	const int32 CharactersNum = AllCharacters.Num();
	const int32 EmptySlotNum = (CharactersNum % ColumnNum != 0) ? ColumnNum - (CharactersNum % ColumnNum) : 0;
	const int32 SlotNum = CharactersNum + EmptySlotNum;

	for (int32 Index = 0; Index < SlotNum; Index++)
	{
		UCharacterIndexSlot* IndexSlot = CreateWidget<UCharacterIndexSlot>(GetWorld(), CharacterIndexSlotClass.Get());
		if (!ensure(IndexSlot))
		{
			return;
		}

		UUniformGridSlot* GridSlot = CharacterGridPanel->AddChildToUniformGrid(IndexSlot, Index / ColumnNum, Index % ColumnNum);
		if (GridSlot)
		{
			GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
		}

		if (AllCharacters.IsValidIndex(Index))
		{
			//IndexSlot->Init(AllCharacters[Index]->CharacterKey);
			IndexSlot->InitByKey(AllCharacters[Index]->CharacterKey);
		}
		else
		{
			IndexSlot->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UInGameIndexWidget::OnOpen()
{
	Super::OnOpen();

	Refresh();
}

void UInGameIndexWidget::Refresh()
{
	const int32 SlotNum = CharacterGridPanel->GetChildrenCount();

	for (int32 Index = 0; Index < SlotNum; Index++)
	{
		UCharacterIndexSlot* IndexSlot = Cast<UCharacterIndexSlot>(CharacterGridPanel->GetChildAt(Index));
		if (IndexSlot)
		{
			IndexSlot->RefreshSlot();
		}
	}
}

void UCharacterIndexSlot::NativeConstruct()
{
	Super::NativeConstruct();

	/*if (SelectSlotButton)
	{
		SelectSlotButton->OnClicked.AddUniqueDynamic(this, &UCharacterIndexSlot::OnSelectCharacterButtonClicked);
	}

	if (SlotMaterial)
	{
		SlotMaterialInstance = UMaterialInstanceDynamic::Create(SlotMaterial, this);
	}*/
}

void UCharacterIndexSlot::OnSelectButtonClicked()
{
}

void UCharacterIndexSlot::Init(int32 InCharacterKey)
{
	/*const FCharacterInfo* CharacterInfo = CMS::GetCharacterDataTable(InCharacterKey);
	if (!ensure(CharacterInfo))
	{
		return;
	}

	CharacterKey = InCharacterKey;

	if (ProfileImage)
	{
		if (CharacterInfo->CharacterIndexProfile && SlotMaterialInstance)
		{
			SlotMaterialInstance->SetTextureParameterValue("TextureParam", CharacterInfo->CharacterIndexProfile);

			ProfileImage->SetBrushFromMaterial(SlotMaterialInstance);
		}
	}

	if (NameText)
	{
		NameText->SetText(FText::FromName(CharacterInfo->CharacterName));
	}

	if (BackgroundImage)
	{
		FLinearColor Color;

		switch (CharacterInfo->CharacterRank)
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
	}*/
}

void UCharacterIndexSlot::RefreshSlot()
{
	APokeCollectionCharacter* Player = Cast<APokeCollectionCharacter>(GetOwningPlayerPawn());
	if (ensure(Player))
	{
		bEnabled = Player->IsCompleteIndexCharacter(CharacterKey);

		/*if (SlotMaterialInstance)
		{
			SlotMaterialInstance->SetScalarParameterValue("GrayParam", bEnabled ? 0 : 1);
		}*/

		if (ContentInfoBox)
		{
			ContentInfoBox->SetVisibility(bEnabled ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
		}

		if (ProfileImage)
		{
			ProfileImage->SetColorAndOpacity(bEnabled ? FLinearColor(1, 1, 1) : FLinearColor(0, 0, 0));
		}
	}
}
