// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameCharacterInfoWidget.h"

#include "Button.h"
#include "Image.h"
#include "ScrollBox.h"
#include "TextBlock.h"
#include "WidgetSwitcher.h"
#include "Materials/MaterialInstanceDynamic.h"

#include "PokeCharacter.h"
#include "PokeCollectionCharacter.h"
#include "PokeCollectionHUD.h"


void UInGameCharacterInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInGameCharacterInfoWidget::OnOpen()
{
	Super::OnOpen();

	if (!ensure(CatergoryScrollBox))
	{
		return;
	}

	if (!ensure(InfoContentsBox))
	{
		return;
	}

	APokeCollectionCharacter* Player = GetPlayer();
	if (!ensure(Player))
	{
		return;
	}


	APokeCharacter* SelectedCharacter = Player->GetCharacterByID(SelectedCharacterID);
	if (!ensure(SelectedCharacter))
	{
		return;
	}

	CatergoryScrollBox->ClearChildren();
	InfoContentsBox->ClearChildren();
	InfoContents.Empty();

	for (int32 ContextIndex = 0; ContextIndex < InfoContentWidgets.Num(); ++ContextIndex)
	{
		if (!InfoContentWidgets[ContextIndex].ContentWidgetClass.Get())
		{
			ensure(0);
			return;
		}

		if (!InfoCategoryButtonClass.Get())
		{
			ensure(0);
			break;
		}

		UInfoCategoryButtonWidget* InfoCategoryButton = CreateWidget<UInfoCategoryButtonWidget>(GetOwningPlayer(), InfoCategoryButtonClass.Get());
		if (ensure(InfoCategoryButton))
		{
			InfoCategoryButton->SetSwitcherIndex(ContextIndex);
			InfoCategoryButton->SetCategoryName(InfoContentWidgets[ContextIndex].CategoryName);
			CatergoryScrollBox->AddChild(InfoCategoryButton);
		}

		UCharacterInfoContentWidget* InfoContentWidget = CreateWidget<UCharacterInfoContentWidget>(GetOwningPlayer(), InfoContentWidgets[ContextIndex].ContentWidgetClass.Get());
		if (ensure(InfoContentWidget))
		{
			InfoContentWidget->SetSelectedCharacter(SelectedCharacter);

			SetBackgroundColor(SelectedCharacter->GetCharacterRank());

			InfoContentsBox->AddChild(InfoContentWidget);
			InfoContents.Add(InfoContentWidget);
		}

	}

	InfoContentsBox->SetActiveWidgetIndex(SelectedContentWidgetIndex);

	if (TypeMaterial)
	{
		if (!Type1MaterialInstance)
		{
			Type1MaterialInstance = UMaterialInstanceDynamic::Create(TypeMaterial, this);
		}
		if (!Type2MaterialInstance)
		{
			Type2MaterialInstance = UMaterialInstanceDynamic::Create(TypeMaterial, this);
		}
	}

	if (CharacterImage)
	{
		CharacterImage->SetBrushFromTexture(SelectedCharacter->GetCharacterImage());
	}

	if (CharacterName)
	{
		CharacterName->SetText(FText::FromName(SelectedCharacter->GetCharacterName()));
	}

	SetTypeImages(SelectedCharacter->GetCharacterType());
}

void UCharacterInfoContentWidget::NativeConstruct()
{
	Super::NativeConstruct();

	OnOpen();
}

void UCharacterInfoContentWidget::OnOpen()
{
}

void UCharacterInfoContentWidget::SetSelectedCharacter(APokeCharacter* InSelectedCharacter)
{
	CurrentSelectedCharacter = InSelectedCharacter;
}

void UInfoCategoryButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (CategoryButton)
	{
		CategoryButton->OnClicked.AddUniqueDynamic(this, &UInfoCategoryButtonWidget::OnCategoryButtonClicked);
	}
}

void UInfoCategoryButtonWidget::SetCategoryName(const FText& InCategoryName)
{
	if (!ensure(CategoryNameText))
	{
		return;
	}

	CategoryNameText->SetText(InCategoryName);
}

void UInfoCategoryButtonWidget::OnCategoryButtonClicked()
{
	APokeCollectionHUD* PokeHud = Cast<APokeCollectionHUD>(GetOwningPlayer()->GetHUD());
	if (PokeHud)
	{
		UInGameCharacterInfoWidget* InfoWidget = PokeHud->GetInGameCharacterInfoWidget();
		if (ensure(InfoWidget))
		{
			InfoWidget->SwitchContentWidget(SwitcherIndex);
		}
	}
}

void UInGameCharacterInfoWidget::SwitchContentWidget(int32 InContentIndex)
{
	if (InfoContentsBox)
	{
		InfoContentsBox->SetActiveWidgetIndex(InContentIndex);
		SelectedContentWidgetIndex = InContentIndex;
	}

	if (InfoContents.IsValidIndex(InContentIndex))
	{
		InfoContents[InContentIndex]->OnOpen();
	}
}

void UInGameCharacterInfoWidget::SetTypeImages(CharacterType InCharacterType)
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

void UInGameCharacterInfoWidget::SetBackgroundColor(ERank InRank)
{
	if (RankBackground)
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

		RankBackground->SetColorAndOpacity(Color);
	}
}
