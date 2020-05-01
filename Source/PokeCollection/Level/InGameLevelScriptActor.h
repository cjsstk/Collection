// Copyright Me. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "InGameLevelScriptActor.generated.h"

/**
 * 
 */
UCLASS()
class POKECOLLECTION_API AInGameLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:
	class ABattleManager* GetBattleManager() const { return BattleManager; }

	UFUNCTION()
	void OnWidgetLoaded();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(Transient)
	class ABattleManager* BattleManager = nullptr;
};
