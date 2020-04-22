// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleResultPopUp.h"

#include "Button.h"

#include "BattleManager.h"
#include "PokeCollectionHUD.h"
#include "PokeCore.h"

void UBattleResultPopUp::NativeConstruct()
{
	Super::NativeConstruct();

	if (BackgroundButton)
	{
		BackgroundButton->OnClicked.AddUniqueDynamic(this, &UBattleResultPopUp::OnBackgroundButtonClicked);
	}
}

void UBattleResultPopUp::InitResultInfo(FBattleReward& InBattleReward)
{

}

void UBattleResultPopUp::OnBackgroundButtonClicked()
{
	RemoveFromViewport();

	APokeCollectionHUD* PokeHud = GetOwningPlayer() ? Cast<APokeCollectionHUD>(GetOwningPlayer()->GetHUD()) : nullptr;
	if (PokeHud)
	{
		PokeHud->OpenInGameMakePartyWidget(true);

		UInGameTopStatusBar* TopStatusBar = PokeHud->GetInGameTopStatusBar();
		if (ensure(TopStatusBar))
		{
			TopStatusBar->AddToViewport(1);
		}
	}

	ABattleManager* BattleManager = PokeCore::GetBattleManager(GetWorld());
	if (ensure(BattleManager))
	{
		BattleManager->BattleShutdown();
	}
}
