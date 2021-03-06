﻿// Fill out your copyright notice in the Description page of Project Settings.


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
		CharacterImage->SetBrushFromTexture(HatchingCharacter->CharacterImage.LoadSynchronous());
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
	CloseWidget();
}

void UEggHatchingWidget::OnBackgroundButtonClicked()
{
	CloseWidget();
}

void UEggHatchingWidget::OnHatchingAnimationFinished()
{
	if (BackgroundButton)
	{
		BackgroundButton->SetVisibility(ESlateVisibility::Visible);
	}
}

void UEggHatchingWidget::CloseWidget()
{
	APokeCollectionHUD* PokeHud = GetPokeHud();
	if (ensure(PokeHud))
	{
		UUserWidget* TopStatusBar = PokeHud->GetInGameTopStatusBar();
		if (ensure(TopStatusBar))
		{
			TopStatusBar->AddToViewport(1);
		}

		PokeHud->OnBackButtonClicked(this);
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