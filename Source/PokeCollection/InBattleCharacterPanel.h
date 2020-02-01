// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InBattleCharacterPanel.generated.h"

UCLASS()
class POKECOLLECTION_API AInBattleCharacterPanel : public AActor
{
	GENERATED_BODY()
	
public:	
	AInBattleCharacterPanel();

	int32 GetPanelNum() const { return PanelNum; }
	bool IsEnemyPanel() const { return bIsEnemyPanel; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, Category = "Panel")
	bool bIsEnemyPanel = false;

	UPROPERTY(EditAnywhere, Category = "Panel")
	int32 PanelNum = 0;
};
