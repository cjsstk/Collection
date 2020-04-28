// Copyright Me. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameBattleWidget.generated.h"

UCLASS()
class POKECOLLECTION_API UBattleCharacterSkillSlot : public UUserWidget
{
	GENERATED_BODY()

public:

private:
	UPROPERTY(Transient)
	class ABattleCharacterActor* OwnerCharacterActor = nullptr;

};

/**
 * In Battle widget
 */
UCLASS()
class POKECOLLECTION_API UInGameBattleWidget : public UUserWidget
{
	GENERATED_BODY()

public:

private:
	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* SkillSlotBox = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UBattleCharacterSkillSlot> CharacterSkillSlotClass;
};
