// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameTopStatusBar.h"

#include "Button.h"
#include "Image.h"
#include "TextBlock.h"

#include "PokeCollectionCharacter.h"
#include "PokeCollectionHUD.h"

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
		const int32 BerryChargingAmount = Player->GetBerryChargingAmount();
		const int32 MoneyChargingAmount = Player->GetMoneyChargingAmount();
		const int32 BerryChargingIntervalMinutes = Player->GetBerryChargingIntervalMinutes();
		const int32 MoneyChargingIntervalMinutes = Player->GetMoneyChargingIntervalMinutes();

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
		if (BerryChargingText)
		{
			FString BerryCharging;
			BerryCharging = "+" + FString::FormatAsNumber(BerryChargingAmount) + "/" + FString::FormatAsNumber(BerryChargingIntervalMinutes) + "m";
			BerryChargingText->SetText(FText::FromString(BerryCharging));
		}
		if (MoneyChargingText)
		{
			FString MoneyCharging;
			MoneyCharging = "+" + FString::FormatAsNumber(MoneyChargingAmount) + "/" + FString::FormatAsNumber(MoneyChargingIntervalMinutes) + "m";
			MoneyChargingText->SetText(FText::FromString(MoneyCharging));
		}
	}

	APokeCollectionHUD* PokeHud = Cast<APokeCollectionHUD>(GetOwningPlayer()->GetHUD());
	if (PokeHud)
	{
		UInGameMainWidget* MainWidget = PokeHud->GetInGameMainWidget();
		if (MainWidget && MainWidget->IsInViewport())
		{
			BackButtonImage->SetBrushFromTexture(nullptr);
		}
		else
		{
			if (BackButtonTexture)
			{
				BackButtonImage->SetBrushFromTexture(BackButtonTexture);
			}
		}
	}
}

void UInGameTopStatusBar::OnBack()
{
	OnBackButtonClicked.Broadcast();
}
