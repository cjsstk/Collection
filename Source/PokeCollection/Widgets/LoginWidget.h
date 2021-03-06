// Copyright Me. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Net/HttpActor.h"
#include "LoginWidget.generated.h"

/**
 * 
 */
UCLASS()
class POKECOLLECTION_API ULoginWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void OnLoginResponsed(FHttpRequestPtr Request, TSharedPtr<FJsonObject> ResponceJson, bool bWasSuccessful);
	void OnRegistResponsed(FHttpRequestPtr Request, TSharedPtr<FJsonObject> ResponceJson, bool bWasSuccessful);
	void OnRegistQuestsResponsed(FHttpRequestPtr Request, TSharedPtr<FJsonObject> ResponceJson, bool bWasSuccessful);

	UFUNCTION()
	void OnLoginButtonClick();

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* GuestLoginButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* GuestLoginText = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* StartText = nullptr;

	bool bShouldRegist = false;
	bool bCanStartGame = false;
};
