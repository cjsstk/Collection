// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameIndexWidget.h"

#include "Button.h"
#include "UniformGridPanel.h"
#include "UniformGridSlot.h"
#include "ScrollBox.h"
#include "Image.h"
#include "TextBlock.h"
#include "MaterialInstanceDynamic.h"

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
			//GridSlot->SetVerticalAlignment(VAlign_Fill);
		}

		if (AllCharacters.IsValidIndex(Index))
		{
			IndexSlot->Init(AllCharacters[Index]->CharacterKey);
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

	if (SelectSlotButton)
	{
		SelectSlotButton->OnClicked.AddUniqueDynamic(this, &UCharacterIndexSlot::OnSelectCharacterButtonClicked);
	}
}

void UCharacterIndexSlot::Init(int32 InCharacterKey)
{
	const FCharacterInfo* CharacterInfo = CMS::GetCharacterDataTable(InCharacterKey);
	if (!ensure(CharacterInfo))
	{
		return;
	}

	CharacterKey = InCharacterKey;

	if (ProfileImage)
	{
		ProfileImage->SetBrushFromTexture(CharacterInfo->CharacterProfile);

		UMaterialInstanceDynamic* matInstance_ = UMaterialInstanceDynamic::Create(materials[i], this);

		matInstance_->SetVectorParameterValue(FName(TEXT("_BodyColor")), characterColor);
		ProfileImage->SetBrushFromMaterial(matInstance_);
	}

	if (NameText)
	{
		NameText->SetText(FText::FromName(CharacterInfo->CharacterName));
	}
}

void UCharacterIndexSlot::RefreshSlot()
{
	APokeCollectionCharacter* Player = Cast<APokeCollectionCharacter>(GetOwningPlayerPawn());
	if (ensure(Player))
	{
		bEnabled = Player->IsCompleteIndexCharacter(CharacterKey);
	}
}

void UCharacterIndexSlot::OnSelectCharacterButtonClicked()
{

}
