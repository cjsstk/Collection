// Fill out your copyright notice in the Description page of Project Settings.

#include "PokePlayerController.h"

#include "Framework/Application/SlateApplication.h"
#include "Framework/Application/IInputProcessor.h"
#include "Widgets/TouchEffectWidget.h"

class FTouchEffectInputProcessor : public IInputProcessor
{
public:
	FTouchEffectInputProcessor(APokePlayerController* InOwner)
		: Owner(InOwner)
	{
	}

	void SetOwner(APokePlayerController* InOwner)
	{
		Owner = InOwner;
	}

	virtual ~FTouchEffectInputProcessor() = default;

	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override
	{
	}

	virtual bool HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent)
	{
		if (!Owner || !MouseEvent.IsTouchEvent())
		{
			return false;
		}

		Owner->OnScreenTouched(MouseEvent);
		return false;
	}

private:
	APokePlayerController* Owner;
};


void APokePlayerController::AddBattleLogString(const FString& InBattleLogString)
{
	BattleLogString += InBattleLogString;

	BattleLogString += TEXT("\n");
}

void APokePlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	bEnableTouchEvents = true;
	bShowMouseCursor = true;

	if (TouchEffectWidgetClass.Get())
	{
		TouchEffectWidget = CreateWidget<UTouchEffectWidget>(this, TouchEffectWidgetClass.Get());
		if (!TouchEffectWidget)
		{
			ensure(0);
		}

		TouchEffectWidget->AddToViewport(4);
		TouchEffectWidget->SetPositionInViewport(FVector2D(0, 0));
		TouchEffectWidget->SetRenderOpacity(0);

		InputProcessor = MakeShared<FTouchEffectInputProcessor>(this);
		if (InputProcessor)
		{
			FSlateApplication::Get().RegisterInputPreProcessor(InputProcessor, 0);
		}
	}

}

void APokePlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (InputProcessor)
	{
		FSlateApplication::Get().UnregisterInputPreProcessor(InputProcessor);
		InputProcessor = nullptr;
	}

	Super::EndPlay(EndPlayReason);
}

void APokePlayerController::OnScreenTouched(const FPointerEvent& MouseEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("InputTouch"));

	if (!TouchEffectWidget)
	{
		return;
	}

	TouchEffectWidget->OnScreenTouched(MouseEvent);
}