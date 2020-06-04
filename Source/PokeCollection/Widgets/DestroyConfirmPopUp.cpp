// Copyright Me. All Rights Reserved.


#include "DestroyConfirmPopUp.h"

#include "PokeCollectionCharacter.h"
#include "PokeCollectionHUD.h"
#include "Widgets/InGameSummonWidget.h"

void UDestroyCharacterConfirmPopUp::OnConfirmed()
{
	Super::OnConfirmed();

	APokeCollectionCharacter* Player = Cast<APokeCollectionCharacter>(GetOwningPlayerPawn());
	if (!ensure(Player))
	{
		return;
	}

	APokeCollectionHUD* PokeHud = Cast<APokeCollectionHUD>(GetOwningPlayer()->GetHUD());
	if (!PokeHud)
	{
		return;
	}

	UInGameSummonWidget* SummonWidget = PokeHud->GetInGameSummonWidget();
	if (!SummonWidget)
	{
		return;
	}

	Player->DeleteCharacters(Player->SeletedDestroyCharacterIDs);

	SummonWidget->SwitchContentWidget(SummonWidget->GetSelectedContentWidgetIndex());

	RemoveFromViewport();
}

void UDestroyEquipmentConfirmPopUp::OnConfirmed()
{
	APokeCollectionCharacter* Player = Cast<APokeCollectionCharacter>(GetOwningPlayerPawn());
	if (!ensure(Player))
	{
		return;
	}

	APokeCollectionHUD* PokeHud = Cast<APokeCollectionHUD>(GetOwningPlayer()->GetHUD());
	if (!PokeHud)
	{
		return;
	}

	UInGameSummonWidget* SummonWidget = PokeHud->GetInGameSummonWidget();
	if (!SummonWidget)
	{
		return;
	}

	Player->DeleteEquipments(Player->SeletedDestroyEquipmentIDs);

	SummonWidget->SwitchContentWidget(SummonWidget->GetSelectedContentWidgetIndex());

	RemoveFromViewport();
}
