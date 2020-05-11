// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameCharacterInfoWidget.h"

#include "Button.h"
#include "Image.h"
#include "ScrollBox.h"
#include "TextBlock.h"
#include "WidgetSwitcher.h"

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
			APokeCollectionCharacter* Player = GetPlayer();
			if (ensure(Player))
			{
				APokeCharacter* SelectedCharacter = Player->GetCharacterByID(SelectedCharacterID);
				if (ensure(SelectedCharacter))
				{
					InfoContentWidget->SetSelectedCharacter(SelectedCharacter);

					SetBackgroundColor(SelectedCharacter->GetCharacterRank());
				}
			}

			InfoContentsBox->AddChild(InfoContentWidget);
			InfoContents.Add(InfoContentWidget);
		}

	}

	InfoContentsBox->SetActiveWidgetIndex(SelectedContentWidgetIndex);
}

void UCharacterInfoContentWidget::NativeConstruct()
{
	Super::NativeConstruct();

	OnOpen();
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
