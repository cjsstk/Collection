// Copyright Me. All Rights Reserved.


#include "LoginWidget.h"

#include "Button.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "Kismet/GameplayStatics.h"

#include "PokeCore.h"

void ULoginWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AHttpActor* HttpActor = PokeCore::GetHttpActor(GetWorld());
	if (!ensure(HttpActor))
	{
		return;
	}

	HttpActor->OnHttpResponseReceived.BindUObject(this, &ULoginWidget::OnLoginResponsed);

	FString DeviceId = FGenericPlatformMisc::GetDeviceId();
	HttpActor->RequestLogin(DeviceId);

	if (GuestLoginButton)
	{
		GuestLoginButton->OnClicked.AddUniqueDynamic(this, &ULoginWidget::OnLoginButtonClick);
	}
}

void ULoginWidget::OnLoginResponsed(FHttpRequestPtr Request, TSharedPtr<FJsonObject> ResponceJson, bool bWasSuccessful)
{
	if (!ResponceJson)
	{
		return;
	}


}

void ULoginWidget::OnLoginButtonClick()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("InGameLevel")));
}
