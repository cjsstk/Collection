// Copyright Me. All Rights Reserved.


#include "TouchEffectWidget.h"

#include "Engine.h"
#include "Framework/Application/SlateApplication.h"
#include "Image.h"
#include "Materials/MaterialInstanceDynamic.h"

void UTouchEffectWidget::OnScreenTouched(const FPointerEvent& MouseEvent)
{
	SetRenderOpacity(1);
	TouchEffectMatIndex = 0;
	ChangeEffectImage();

	if (UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport())
	{
		const FGeometry& CachedGeometry = ViewportClient->GetGameViewportWidget()->GetCachedGeometry();
		FVector2D NewPosition = CachedGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
		NewPosition.X *= CachedGeometry.Scale;
		NewPosition.Y *= CachedGeometry.Scale;

		FVector2D EffectHalfSize = GetCachedGeometry().GetAbsoluteSize() * 0.5f;

		SetPositionInViewport((NewPosition - EffectHalfSize).IntPoint());
	}
}

void UTouchEffectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (TouchEffectMaterial)
	{
		TouchEffectMaterialInstance = UMaterialInstanceDynamic::Create(TouchEffectMaterial, this);
	}
}

void UTouchEffectWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!FMath::IsNearlyEqual(GetRenderOpacity(), 1.0f))
	{
		return;
	}

	if (TouchEffectMatIndex > MaxTouchEffectSpriteNum)
	{
		SetRenderOpacity(0);
		return;
	}


	TouchEffectDelayAgeSeconds += InDeltaTime;

	if (TouchEffectDelayAgeSeconds > TouchEffectDelaySeconds)
	{
		TouchEffectMatIndex++;

		ChangeEffectImage();

		TouchEffectDelayAgeSeconds = 0.0f;
	}
}

void UTouchEffectWidget::ChangeEffectImage()
{
	if (!TouchEffectImage)
	{
		return;
	}

	if (!TouchEffectMaterialInstance)
	{
		return;
	}


	int32 EffectColumnIndex = TouchEffectMatIndex % TouchEffectColumnNum;
	int32 EffectRawIndex = TouchEffectMatIndex / TouchEffectRawNum;

	TouchEffectMaterialInstance->SetScalarParameterValue("ColumnIndexParam", EffectColumnIndex);
	TouchEffectMaterialInstance->SetScalarParameterValue("RowIndexParam", EffectRawIndex);

	TouchEffectImage->SetBrushFromMaterial(TouchEffectMaterialInstance);
}
