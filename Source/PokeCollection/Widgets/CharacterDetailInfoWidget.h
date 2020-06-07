// Copyright Me. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/InGameCharacterInfoWidget.h"
#include "CharacterDetailInfoWidget.generated.h"

/**
 * 캐릭터 상세 화면 위젯
 */
UCLASS()
class POKECOLLECTION_API UCharacterDetailInfoWidget : public UCharacterInfoContentWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void OnOpen() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CharacterLevel = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentExp = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MaxExp = nullptr;

	/** Status */
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HealthPoint = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MaxHealthPoint = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Attack = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Defence = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SpecialAttack = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SpecialDefence = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Speed = nullptr;

	/** */
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AttackRange = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ConsumeBerryAmount = nullptr;

};
