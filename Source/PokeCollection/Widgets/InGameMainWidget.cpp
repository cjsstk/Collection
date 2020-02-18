// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMainWidget.h"

#include "Button.h"
#include "WidgetLayoutLibrary.h"

#include "PokeCollectionCharacter.h"
#include "PokeCollectionGameMode.h"
#include "PokeCollectionHUD.h"
#include "Widgets/InGameTopStatusBar.h"

void UInGameWidget::NativeConstruct()
{
	APokeCollectionHUD* Hud = Cast<APokeCollectionHUD>(GetOwningPlayer()->GetHUD());
	if (ensure(Hud))
	{
		UInGameTopStatusBar* TopStatusBar = Hud->GetInGameTopStatusBar();
		if (ensure(TopStatusBar))
		{
			TopStatusBar->OnBackButtonClicked.AddUniqueDynamic(this, &UInGameWidget::OnBack);
		}
	}
}

void UInGameWidget::OnBack()
{
	if (!IsInViewport())
	{
		return;
	}

	APokeCollectionHUD* Hud = GetPokeHud();
	if (Hud)
	{
		Hud->OnBackButtonClicked(this);
	}

	//if (PrevWidget)
	//{
	//	PrevWidget->AddToViewport();
	//}
	//else
	//{
	//	ensure(0);
	//	// Profile?
	//}
}

APokeCollectionCharacter* UInGameWidget::GetPlayer() const
{
	return Cast<APokeCollectionCharacter>(GetOwningPlayerPawn());
}

class APokeCollectionHUD* UInGameWidget::GetPokeHud() const
{
	return GetOwningPlayer() ? Cast<APokeCollectionHUD>(GetOwningPlayer()->GetHUD()) : nullptr;
}

void UInGameMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!CheckAllButtonsExist())
	{
		ensure(0);
	}

	if (TempStartButton)
	{
		TempStartButton->OnClicked.AddUniqueDynamic(this, &UInGameMainWidget::OnStartButtonClicked);
	}

	/*ShopButton->OnClicked.AddUniqueDynamic(this, &UInGameMainWidget::OnStartButtonClicked);
	CharacterBoxButton->OnClicked.AddUniqueDynamic(this, &UInGameMainWidget::OnStartButtonClicked);
	InventoryButton->OnClicked.AddUniqueDynamic(this, &UInGameMainWidget::OnStartButtonClicked);
	SecretBaseButton->OnClicked.AddUniqueDynamic(this, &UInGameMainWidget::OnStartButtonClicked);
	WalkButton->OnClicked.AddUniqueDynamic(this, &UInGameMainWidget::OnStartButtonClicked);
	MissionButton->OnClicked.AddUniqueDynamic(this, &UInGameMainWidget::OnStartButtonClicked);
	DayCareButton->OnClicked.AddUniqueDynamic(this, &UInGameMainWidget::OnStartButtonClicked);*/
	PartyMakeButton->OnClicked.AddUniqueDynamic(this, &UInGameMainWidget::OnPartyMakeButtonClicked);
	AdventureButton->OnClicked.AddUniqueDynamic(this, &UInGameMainWidget::OnAdventureButtonClicked);

}

void UInGameMainWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (TempStartButton)
	{
		TempStartButton->OnClicked.RemoveDynamic(this, &UInGameMainWidget::OnStartButtonClicked);
	}
}

void UInGameMainWidget::OnStartButtonClicked()
{
	UWorld* World = GetWorld();
	if (World)
	{
		APokeCollectionGameMode* GameMode = Cast<APokeCollectionGameMode>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			GameMode->BattleStart();
		}
	}

	SetVisibility(ESlateVisibility::Collapsed);
}

void UInGameMainWidget::OnPartyMakeButtonClicked()
{
}

void UInGameMainWidget::OnAdventureButtonClicked()
{
	APokeCollectionHUD* Hud = GetPokeHud();
	if (Hud)
	{
		Hud->OpenInGameAdventureWidget();
	}

}

bool UInGameMainWidget::CheckAllButtonsExist()
{
	bool bIsExist = true;

	bIsExist = bIsExist && ShopButton;
	bIsExist = bIsExist && CharacterBoxButton;
	bIsExist = bIsExist && InventoryButton;
	bIsExist = bIsExist && SecretBaseButton;
	bIsExist = bIsExist && WalkButton;
	bIsExist = bIsExist && MissionButton;
	bIsExist = bIsExist && DayCareButton;
	bIsExist = bIsExist && PartyMakeButton;
	bIsExist = bIsExist && AdventureButton;

	return bIsExist;
}
