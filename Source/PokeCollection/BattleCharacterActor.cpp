// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleCharacterActor.h"

#include "Components/BoxComponent.h"
#include "PaperFlipbookComponent.h"

ABattleCharacterActor::ABattleCharacterActor()
{
	BodyBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BodyBoxComponent"));
	SetRootComponent(BodyBoxComponent);

	RenderComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("FlipbookComponent"));
	RenderComponent->SetupAttachment(RootComponent);


	//RootComponent->SetWorldRotation(FRotator(0, 90, 0));
}
