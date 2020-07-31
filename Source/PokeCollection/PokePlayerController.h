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

	void OnScreenTouched(const FPointerEvent& MouseEvent);

private:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

private:
	FString BattleLogString;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UTouchEffectWidget> TouchEffectWidgetClass;

	UPROPERTY(Transient)
	class UTouchEffectWidget* TouchEffectWidget = nullptr;

	TSharedPtr<class FTouchEffectInputProcessor> InputProcessor;
};
