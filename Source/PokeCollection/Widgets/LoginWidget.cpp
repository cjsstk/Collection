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

	HttpActor->OnHttpLoginResponseReceived.BindUObject(this, &ULoginWidget::OnLoginResponsed);
	HttpActor->OnHttpRegistResponseReceived.BindUObject(this, &ULoginWidget::OnRegistResponsed);
	HttpActor->OnHttpRegistQuestsResponseReceived.BindUObject(this, &ULoginWidget::OnRegistQuestsResponsed);

	FHttpRequestParams Params;
	Params.RequestType = EHttpRequestType::Login;
	Params.MemberID = PokeCore::DeviceId;

	HttpActor->Request(Params);

	if (GuestLoginButton)
	{
		GuestLoginButton->OnClicked.AddUniqueDynamic(this, &ULoginWidget::OnLoginButtonClick);
	}

	GuestLoginText->SetVisibility(ESlateVisibility::Collapsed);
	StartText->SetVisibility(ESlateVisibility::Collapsed);
}

void ULoginWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bCanStartGame)
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("InGameLevel")));
		bCanStartGame = false;
	}
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

void ULoginWidget::OnRegistResponsed(FHttpRequestPtr Request, TSharedPtr<FJsonObject> ResponceJson, bool bWasSuccessful)
{
	if (!ResponceJson)
	{
		return;
	}

	int32 recievedCode = ResponceJson->GetIntegerField("code");
	if (recievedCode == 200)
	{
		AHttpActor* HttpActor = PokeCore::GetHttpActor(GetWorld());
		if (!ensure(HttpActor))
		{
			return;
		}

		FHttpRequestParams Params;
		Params.RequestType = EHttpRequestType::RegistQuests;
		Params.MemberID = PokeCore::DeviceId;

		HttpActor->Request(Params);
		//bCanStartGame = true;
	}
	else
	{
		// 생성 실패
		// DB error
		ensure(0);
	}
}

void ULoginWidget::OnRegistQuestsResponsed(FHttpRequestPtr Request, TSharedPtr<FJsonObject> ResponceJson, bool bWasSuccessful)
{
	if (!ResponceJson)
	{
		ensure(0);
		return;
	}

	int32 recievedCode = ResponceJson->GetIntegerField("code");
	if (recievedCode == 200)
	{
		bCanStartGame = true;
	}
	else
	{
		// 생성 실패
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

		FHttpRequestParams Params;
		Params.RequestType = EHttpRequestType::Regist;
		Params.MemberID = PokeCore::DeviceId;

		HttpActor->Request(Params);
		return;
	}

	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("InGameLevel")));
}
