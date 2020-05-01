// Copyright Me. All Rights Reserved.


#include "LoginLevelScriptActor.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

#include "PokeCollectionHUD.h"
#include "Widgets/LoginWidget.h"

void ALoginLevelScriptActor::BeginPlay()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		APokeCollectionHUD* PokeHud = Cast<APokeCollectionHUD>(PC->GetHUD());
		if (PokeHud)
		{
			PokeHud->OnWidgetLoaded.AddUniqueDynamic(this, &ALoginLevelScriptActor::OnWidgetLoaded);
		}
	}
}

void ALoginLevelScriptActor::OnWidgetLoaded()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		APokeCollectionHUD* PokeHud = Cast<APokeCollectionHUD>(PC->GetHUD());
		if (PokeHud)
		{
			if (PokeHud->GetLoginWidget())
			{
				PokeHud->GetLoginWidget()->AddToViewport();
			}
		}
	}
}
