// Copyright Me. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/InGameCharacterInfoWidget.h"
#include "CharacterDetailInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class POKECOLLECTION_API UCharacterDetailInfoWidget : public UCharacterInfoContentWidget
{
	GENERATED_BODY()
public:
	virtual void OnOpen() override;

private:
	void SetTypeImages(CharacterType InCharacterType);

	UPROPERTY(meta = (BindWidget))
	class UImage* CharacterImage = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CharacterName = nullptr;

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
	class UImage* RankBackground = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UImage* Type1 = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UImage* Type2 = nullptr;
};
