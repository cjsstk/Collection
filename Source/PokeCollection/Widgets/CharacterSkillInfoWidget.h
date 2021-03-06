// Copyright Me. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/InGameCharacterInfoWidget.h"
#include "PokeSkill.h"
#include "CharacterSkillInfoWidget.generated.h"


UCLASS()
class POKECOLLECTION_API USkillInfoSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	//virtual void OnOpen() override;
	void InitSkillInfo(const InitSkillParams& Params);

	void SetActivateSlot(bool bActivate);

	UFUNCTION()
	void OnSkillUpgradeButtonClicked();

private:
	UPROPERTY(meta = (BindWidget))
	class UImage* SkillIcon = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SkillDamageText  = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SkillNameText = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SkillDescText = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* SkillUpgradeButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UOverlay* DeactiveMessage = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ActiveLevelMessage = nullptr;

	int32 SkillKey = 0;
	int32 SkillIndex = 0;
};

/**
 * 캐릭터 스킬 정보가 나오는 위젯
 */
UCLASS()
class POKECOLLECTION_API UCharacterSkillInfoWidget : public UCharacterInfoContentWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void OnOpen() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* SkillSlotBox = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USkillInfoSlot> SkillInfoSlotClass = nullptr;


};
