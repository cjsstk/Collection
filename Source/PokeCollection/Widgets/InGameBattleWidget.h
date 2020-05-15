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
	void SetOwnerBattleCharacter(class ABattleCharacterActor& InBattleCharacter);

	UFUNCTION()
	void OnUseSkillButtonClicked();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	void InitSlotInfo();

	UPROPERTY(meta = (BindWidget))
	class UImage* CharacterIcon = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* SkillGaugeBar = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UImage* SkillReady = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* UseSkillButton = nullptr;

	UPROPERTY(Transient)
	class ABattleCharacterActor* OwnerBattleCharacter;

	UPROPERTY(Transient)
	class UPokeSkill* ActiveSkill = nullptr;

};

UCLASS()
class POKECOLLECTION_API UBattleLogWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnBattleLogAdded(FString& NewBattleLog);

private:
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* BattleLogBox = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> LogTextClass;
};

/**
 * In Battle widget
 */
UCLASS()
class POKECOLLECTION_API UInGameBattleWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void InitBattleCharacters(const TArray<class ABattleCharacterActor*>& InPlayerBattleCharacter);

	UFUNCTION()
	void OnLogButtonClicked();

private:
	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* SkillSlotBox = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UBattleLogWidget* LogWidget = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* LogButton = nullptr;

	UPROPERTY(Transient)
	TArray<class ABattleCharacterActor*> PlayerBattleCharacters;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UBattleCharacterSkillSlot> CharacterSkillSlotClass;
};
