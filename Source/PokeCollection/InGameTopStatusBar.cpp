// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameTopStatusBar.h"

#include "Button.h"
#include "TextBlock.h"

#include "PokeCollectionCharacter.h"

void UInGameTopStatusBar::NativeConstruct()
{
	Super::NativeConstruct();

	if (BackButton)
	{
		BackButton->OnClicked.AddUniqueDynamic(this, &UInGameTopStatusBar::OnBack);
	}

}

void UInGameTopStatusBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	APokeCollectionCharacter* Player = Cast<APokeCollectionCharacter>(GetOwningPlayerPawn());
	if (ensure(Player))
	{
		const int32 BerryAmount = Player->GetBerryAmount();
		const int32 MoneyAmount = Player->GetMoneyAmount();
		const int32 StardustAmount = Player->GetStardustAmount();

		if (BerryAmountText)
		{
			BerryAmountText->SetText(FText::AsNumber(BerryAmount));
		}
		if (MoneyAmountText)
		{
			MoneyAmountText->SetText(FText::AsNumber(MoneyAmount));
		}
		if (StardustAmountText)
		{
			StardustAmountText->SetText(FText::AsNumber(StardustAmount));
		}
	}

}

void UInGameTopStatusBar::OnBack()
{
	OnBackButtonClicked.Broadcast();
}
