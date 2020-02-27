// Fill out your copyright notice in the Description page of Project Settings.


#include "MakePartyCharacterPanel.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PaperFlipbookComponent.h"

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

void AMakePartyCharacterPanel::BeginPlay()
{
	Super::BeginPlay();
	
}

