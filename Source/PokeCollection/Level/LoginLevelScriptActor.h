// Copyright Me. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "LoginLevelScriptActor.generated.h"

/**
 * 
 */
UCLASS()
class POKECOLLECTION_API ALoginLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void OnWidgetLoaded();

protected:
	virtual void BeginPlay() override;

};
