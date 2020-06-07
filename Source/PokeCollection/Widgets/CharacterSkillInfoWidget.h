// Copyright Me. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/InGameCharacterInfoWidget.h"
#include "CharacterSkillInfoWidget.generated.h"

UCLASS()
class POKECOLLECTION_API USkillInfoSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	//virtual void NativeConstruct() override;
	//virtual void OnOpen() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UImage* SkillIcon = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SkillDamageText  = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SkillDescText = nullptr;


};

/**
 * ĳ���� ��ų ������ ������ ����
 */
UCLASS()
class POKECOLLECTION_API UCharacterSkillInfoWidget : public UCharacterInfoContentWidget
{
	GENERATED_BODY()
	
public:
	//virtual void NativeConstruct() override;
	//virtual void OnOpen() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* SkillSlotBox = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USkillInfoSlot> SkillInfoSlotClass = nullptr;


};
