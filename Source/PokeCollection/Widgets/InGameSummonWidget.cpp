// Copyright Me. All Rights Reserved.


#include "InGameSummonWidget.h"

#include "TextBlock.h"
#include "Button.h"
#include "ScrollBox.h"
#include "WidgetSwitcher.h"

#include "PokeCollectionHUD.h"
#include "Widgets/InGameBoxWidget.h"

void UInGameSummonWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UInGameSummonWidget::OnOpen()
{
	Super::OnOpen();

	if (!ensure(CatergoryScrollBox))
	{
		return;
	}

	if (!ensure(SummonContentsBox))
	{
		return;
	}

	CatergoryScrollBox->ClearChildren();
	SummonContentsBox->ClearChildren();
	SummonContents.Empty();

	for (int32 ContextIndex = 0; ContextIndex < SummonContentWidgets.Num(); ++ContextIndex)
	{
		if (!SummonContentWidgets[ContextIndex].ContentWidgetClass.Get())
		{
			ensure(0);
			return;
		}

		if (!SummonCategoryButtonClass.Get())
		{
			ensure(0);
			break;
		}

		USummonCategoryButtonWidget* SummonCategoryButton = CreateWidget<USummonCategoryButtonWidget>(GetOwningPlayer(), SummonCategoryButtonClass.Get());
		if (ensure(SummonCategoryButton))
		{
			SummonCategoryButton->SetSwitcherIndex(ContextIndex);
			SummonCategoryButton->SetCategoryName(SummonContentWidgets[ContextIndex].CategoryName);
			CatergoryScrollBox->AddChild(SummonCategoryButton);
		}

		UUserWidget* InfoContentWidget = CreateWidget<UUserWidget>(GetOwningPlayer(), SummonContentWidgets[ContextIndex].ContentWidgetClass.Get());
		if (ensure(InfoContentWidget))
		{
			SummonContentsBox->AddChild(InfoContentWidget);
			SummonContents.Add(InfoContentWidget);
		}

	}

	SummonContentsBox->SetActiveWidgetIndex(0);
}

void UInGameSummonWidget::SwitchContentWidget(int32 InContentIndex)
{
	if (SummonContentsBox)
	{
		SummonContentsBox->SetActiveWidgetIndex(InContentIndex);
		SelectedContentWidgetIndex = InContentIndex;
	}

	if (SummonContents.IsValidIndex(InContentIndex))
	{
		UBoxContentWidget* BoxWidget = Cast<UBoxContentWidget>(SummonContents[InContentIndex]);
		if (BoxWidget)
		{
			BoxWidget->OnOpen();
		}
	}
}

void USummonCategoryButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (CategoryButton)
	{
		CategoryButton->OnClicked.AddUniqueDynamic(this, &USummonCategoryButtonWidget::OnCategoryButtonClicked);
	}
}

void USummonCategoryButtonWidget::SetCategoryName(const FText& InCategoryName)
{
	if (!ensure(CategoryNameText))
	{
		return;
	}

	CategoryNameText->SetText(InCategoryName);
}

void USummonCategoryButtonWidget::OnCategoryButtonClicked()
{
	APokeCollectionHUD* PokeHud = Cast<APokeCollectionHUD>(GetOwningPlayer()->GetHUD());
	if (PokeHud)
	{
		UInGameSummonWidget* SummonWidget = PokeHud->GetInGameSummonWidget();
		if (ensure(SummonWidget))
		{
			SummonWidget->SwitchContentWidget(SwitcherIndex);
		}
	}
}
