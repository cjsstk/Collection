// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PokePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class POKECOLLECTION_API APokePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	void AddBattleLogString(const FString& InBattleLogString);

private:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnInputTouch(ETouchIndex::Type FingerIndex, AActor* TouchedActor);

private:
	FString BattleLogString;


};
