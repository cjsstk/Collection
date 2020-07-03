// Copyright Me. All Rights Reserved.


#include "InGameCategoryWidget.h"

#include "TextBlock.h"
#include "Button.h"
#include "ScrollBox.h"
#include "WidgetSwitcher.h"

#include "PokeCollectionHUD.h"

void UInGameCategoryWidget::OnOpen()
{
	Super::OnOpen();

	if (!ensure(CatergoryScrollBox))
	{
		return;
	}

	if (!ensure(ContentsBox))
	{
		return;
	}

	CatergoryScrollBox->ClearChildren();
	ContentsBox->ClearChildren();
	Contents.Empty();

	for (int32 ContextIndex = 0; ContextIndex < ContentWidgets.Num(); ++ContextIndex)
	{
		if (!ContentWidgets[ContextIndex].ContentWidgetClass.Get())
		{
			ensure(0);
			return;
		}

		if (!CategoryButtonClass.Get())
		{
			ensure(0);
			break;
		}

		UCategoryButtonWidget* CategoryButton = CreateWidget<UCategoryButtonWidget>(GetOwningPlayer(), CategoryButtonClass.Get());
		if (ensure(CategoryButton))
		{
			CategoryButton->SetSwitcherIndex(ContextIndex);
			CategoryButton->SetCategoryName(ContentWidgets[ContextIndex].CategoryName);
			CategoryButton->SetCategoryType(CategoryType);
			CatergoryScrollBox->AddChild(CategoryButton);
		}

		UCategoryContentWidget* ContentWidget = CreateWidget<UCategoryContentWidget>(GetOwningPlayer(), ContentWidgets[ContextIndex].ContentWidgetClass.Get());
		if (ensure(ContentWidget))
		{
			ContentsBox->AddChild(ContentWidget);
			Contents.Add(ContentWidget);
		}

	}

	SwitchContentWidget(0);
}

void UInGameCategoryWidget::SwitchContentWidget(int32 InContentIndex)
{
	if (ContentsBox)
	{
		ContentsBox->SetActiveWidgetIndex(InContentIndex);
		SelectedContentWidgetIndex = InContentIndex;

		if (Contents.IsValidIndex(InContentIndex))
		{
			Contents[InContentIndex]->OnOpen();
		}
	}
}

void UCategoryButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (CategoryButton)
	{
		CategoryButton->OnClicked.AddUniqueDynamic(this, &UCategoryButtonWidget::OnCategoryButtonClicked);
	}
}

void UCategoryButtonWidget::OnButtonClicked()
{
	APokeCollectionHUD* PokeHud = Cast<APokeCollectionHUD>(GetOwningPlayer()->GetHUD());
	if (PokeHud)
	{
		UInGameCategoryWidget* CategoryWidget = PokeHud->GetInGameCategoryWidget(CategoryType);
		if (CategoryWidget)
		{
			CategoryWidget->SwitchContentWidget(SwitcherIndex);
		}
	}
}

void UCategoryButtonWidget::SetCategoryName(const FText& InCategoryName)
{
	if (!ensure(CategoryNameText))
	{
		return;
	}

	CategoryNameText->SetText(InCategoryName);
}

void UCategoryButtonWidget::OnCategoryButtonClicked()
{
	OnButtonClicked();
}
