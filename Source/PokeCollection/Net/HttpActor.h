// Copyright Me. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Dom/JsonObject.h"
#include "HttpActor.generated.h"

DECLARE_DELEGATE_ThreeParams(FOnHttpResponseReceived, FHttpRequestPtr, TSharedPtr<FJsonObject>, bool);

UCLASS()
class POKECOLLECTION_API AHttpActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AHttpActor(const class FObjectInitializer& ObjectInitializer);

	UFUNCTION()
	void HttpCall();

	void RequestLogin(const FString& InLoginId);

	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	FOnHttpResponseReceived OnHttpResponseReceived;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	FHttpModule* Http;

};
