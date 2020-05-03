// Copyright Me. All Rights Reserved.


#include "LoginWidget.h"

#include "Button.h"
#include "TextBlock.h"
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
	HttpActor->RequestLogin(PokeCore::DeviceId);

	if (GuestLoginButton)
	{
		GuestLoginButton->OnClicked.AddUniqueDynamic(this, &ULoginWidget::OnLoginButtonClick);
	}

	GuestLoginText->SetVisibility(ESlateVisibility::Collapsed);
	StartText->SetVisibility(ESlateVisibility::Collapsed);
}

void ULoginWidget::OnLoginResponsed(FHttpRequestPtr Request, TSharedPtr<FJsonObject> ResponceJson, bool bWasSuccessful)
{
	if (!ResponceJson)
	{
		return;
	}

	int32 recievedCode = ResponceJson->GetIntegerField("code");
	if (recievedCode == 404)
	{
		GuestLoginText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		bShouldRegist = true;
	}
	else if (recievedCode == 200)
	{
		StartText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		bShouldRegist = false;
	}
	else
	{
		// DB error
		ensure(0);
	}

}

void ULoginWidget::OnLoginButtonClick()
{
	if (bShouldRegist)
	{
		AHttpActor* HttpActor = PokeCore::GetHttpActor(GetWorld());
		if (!ensure(HttpActor))
		{
			return;
		}

		HttpActor->RequestRegist(PokeCore::DeviceId);
	}

	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("InGameLevel")));
}
