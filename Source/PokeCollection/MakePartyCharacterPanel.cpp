// Fill out your copyright notice in the Description page of Project Settings.


#include "MakePartyCharacterPanel.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PaperFlipbookComponent.h"

#include "PokePlayerController.h"
#include "PokeCollectionHUD.h"
#include "Widgets/InGameCharacterBoxWidget.h"

AMakePartyCharacterPanel::AMakePartyCharacterPanel()
{
	//PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);
	BoxComponent->SetWorldRotation(FRotator(0, 270, 90));

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("FlipbookComponent"));
	FlipbookComponent->SetupAttachment(RootComponent);

}

void AMakePartyCharacterPanel::SetFlipbook(UPaperFlipbook* NewFlipbook)
{
	FlipbookComponent->SetFlipbook(NewFlipbook);
}

void AMakePartyCharacterPanel::NotifyActorOnReleased(FKey PressedButton)
{
	UE_LOG(LogTemp, Log, TEXT("NotifyActorOnReleased"));
}

void AMakePartyCharacterPanel::NotifyActorOnInputTouchEnd(const ETouchIndex::Type FingerIndex)
{
	UE_LOG(LogTemp, Log, TEXT("NotifyActorOnInputTouchEnd"));

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		APokeCollectionHUD* PokeHud = Cast<APokeCollectionHUD>(PC->GetHUD());
		if (PokeHud)
		{
			if (PokeHud->GetInGameCharacterBoxWidget()->IsInViewport())
			{
				return;
			}

			PokeHud->OpenInGameCharacterBoxWidget(bIsEmptySlot);
			UInGameCharacterBoxWidget* CharacterBoxWidget = PokeHud->GetInGameCharacterBoxWidget();
			if (CharacterBoxWidget)
			{
				CharacterBoxWidget->SetSelectedPartySlotNum(SlotNum);
			}
		}
	}
}

void AMakePartyCharacterPanel::BeginPlay()
{
	Super::BeginPlay();
	
}

