// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookActor.h"
#include "BattleCharacterActor.generated.h"


UCLASS()
class POKECOLLECTION_API ABattleCharacterActor : public AActor
{
	GENERATED_BODY()
	
public:
	ABattleCharacterActor();

	class UPaperFlipbookComponent* GetRenderComponent() const { return RenderComponent; }

private:
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BodyBoxComponent;

	UPROPERTY(VisibleAnywhere)
	class UPaperFlipbookComponent* RenderComponent;

};
