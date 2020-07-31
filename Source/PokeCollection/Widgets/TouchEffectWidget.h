// Copyright Me. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TouchEffectWidget.generated.h"

/**
 * 터치 시 나타나는 이펙트
 */
UCLASS()
class POKECOLLECTION_API UTouchEffectWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void OnScreenTouched(const FPointerEvent& MouseEvent);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	void ChangeEffectImage();

	UPROPERTY(meta = (BindWidget))
	class UImage* TouchEffectImage = nullptr;

	UPROPERTY(EditDefaultsOnly)
	class UMaterialInterface* TouchEffectMaterial = nullptr;

	UPROPERTY(EditDefaultsOnly)
	int32 TouchEffectColumnNum = 0;

	UPROPERTY(EditDefaultsOnly)
	int32 TouchEffectRawNum = 0;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxTouchEffectSpriteNum = 0;

	UPROPERTY(EditDefaultsOnly)
	float TouchEffectDelaySeconds = 0.1f;

	UPROPERTY(Transient)
	class UMaterialInstanceDynamic* TouchEffectMaterialInstance = nullptr;

	int32 TouchEffectMatIndex = 0;
	float TouchEffectDelayAgeSeconds = 0.0f;
};
