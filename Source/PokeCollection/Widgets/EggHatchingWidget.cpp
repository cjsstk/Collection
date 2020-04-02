// Fill out your copyright notice in the Description page of Project Settings.


#include "EggHatchingWidget.h"

#include "Button.h"
#include "Image.h"
#include "TextBlock.h"

#include "CMS.h"
#include "PokeCollectionHUD.h"

#define LOCTEXT_NAMESPACE "EggHatching"

void UEggHatchingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (SkipButton)
	{
		SkipButton->OnClicked.AddUniqueDynamic(this, &UEggHatchingWidget::OnSkipButtonClicked);
	}

	if (BackgroundButton)
	{
		BackgroundButton->OnClicked.AddUniqueDynamic(this, &UEggHatchingWidget::OnBackgroundButtonClicked);
		BackgroundButton->SetVisibility(ESlateVisibility::Hidden);
	}

	if (HatchingAnim)
	{
		OnAnimationFinished.BindUFunction(this, FName("OnHatchingAnimationFinished"));
		BindToAnimationFinished(HatchingAnim, OnAnimationFinished);
	}

	APokeCollectionHUD* PokeHud = GetOwningPlayer() ? Cast<APokeCollectionHUD>(GetOwningPlayer()->GetHUD()) : nullptr;
	if (ensure(PokeHud))
	{
		UUserWidget* TopStatusBar = PokeHud->GetInGameTopStatusBar();
		if (ensure(TopStatusBar))
		{
			TopStatusBar->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UEggHatchingWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (SkipButton)
	{
		SkipButton->OnClicked.RemoveDynamic(this, &UEggHatchingWidget::OnSkipButtonClicked);
	}

	if (BackgroundButton)
	{
		BackgroundButton->OnClicked.RemoveDynamic(this, &UEggHatchingWidget::OnBackgroundButtonClicked);
	}

	if (HatchingAnim)
	{
		UnbindAllFromAnimationFinished(HatchingAnim);
	}

	APokeCollectionHUD* PokeHud = GetOwningPlayer() ? Cast<APokeCollectionHUD>(GetOwningPlayer()->GetHUD()) : nullptr;
	if (ensure(PokeHud))
	{
		UUserWidget* TopStatusBar = PokeHud->GetInGameTopStatusBar();
		if (ensure(TopStatusBar))
		{
			TopStatusBar->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UEggHatchingWidget::StartHatching(characterKey InCharacterKey)
{
	const FCharacterInfo* HatchingCharacter = CMS::GetCharacterDataTable(InCharacterKey);
	if (!ensure(HatchingCharacter))
	{
		return;
	}

	if (CharacterImage)
	{
		CharacterImage->SetBrushFromTexture(HatchingCharacter->CharacterImage);
	}

	if (HatchingText)
	{
		FName CharacterName = HatchingCharacter->CharacterName;
		
		FFormatNamedArguments Arguments;
		Arguments.Add(TEXT("HatchingCharacter"), FText::FromName(CharacterName));

		FText HatchingResult = FText::Format(NSLOCTEXT("HatchingResult", "EggHatching", "알에서 {HatchingCharacter}가 부화했습니다!"), Arguments);

		HatchingText->SetText(HatchingResult);
	}

	StartAnim();
}

void UEggHatchingWidget::OnSkipButtonClicked()
{
	RemoveFromViewport();
}

void UEggHatchingWidget::OnBackgroundButtonClicked()
{
	RemoveFromViewport();
}

void UEggHatchingWidget::OnHatchingAnimationFinished()
{
	if (BackgroundButton)
	{
		BackgroundButton->SetVisibility(ESlateVisibility::Visible);
	}
}

void UEggHatchingWidget::StartAnim()
{
	if (ensure(HatchingAnim))
	{
		PlayAnimation(HatchingAnim);
	}
}

#undef LOCTEXT_NAMESPACE