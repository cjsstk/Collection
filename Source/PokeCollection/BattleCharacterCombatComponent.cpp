// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleCharacterCombatComponent.h"

UBattleCharacterCombatComponent::UBattleCharacterCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UBattleCharacterCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UBattleCharacterCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

