// Copyright Me. All Rights Reserved.


#include "InGameLevelScriptActor.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

#include "BattleManager.h"
#include "PokeCollectionHUD.h"

void AInGameLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		APokeCollectionHUD* PokeHud = Cast<APokeCollectionHUD>(PC->GetHUD());
		if (PokeHud)
		{
			PokeHud->OnWidgetLoaded.AddUniqueDynamic(this, &AInGameLevelScriptActor::OnWidgetLoaded);
		}
	}

	BattleManager = Cast<ABattleManager>(UGameplayStatics::GetActorOfClass(this, ABattleManager::StaticClass()));
	if (!BattleManager)
	{
		ensure(0);
	}
}

void AInGameLevelScriptActor::OnWidgetLoaded()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		APokeCollectionHUD* PokeHud = Cast<APokeCollectionHUD>(PC->GetHUD());
		if (PokeHud)
		{
			if (PokeHud->GetInGameMainWidget())
			{
				PokeHud->GetInGameMainWidget()->AddToViewport();
			}

			if (PokeHud->GetInGameTopStatusBar())
			{
				PokeHud->GetInGameTopStatusBar()->AddToViewport(1);
			}

			BattleManager->OnBattleStart.AddUniqueDynamic(PokeHud, &APokeCollectionHUD::OnStartBattle);
			BattleManager->OnBattleShutdown.AddUniqueDynamic(PokeHud, &APokeCollectionHUD::OnShutdownBattle);
		}
	}
}
