// Copyright Me. All Rights Reserved.


#include "HttpActor.h"

#include "Engine.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

const static FString CollectionURL = FString("http://collection.nupa.moe/api/v1/");

using VerbType = FString;
namespace HTTPVerb
{
	const static VerbType GET = FString("GET");
	const static VerbType POST = FString("POST");
	const static VerbType PUT = FString("PUT");
	const static VerbType DELETE = FString("DELETE");
}

AHttpActor::AHttpActor(const class FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	Http = &FHttpModule::Get();
}

void AHttpActor::HttpCall()
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	JsonObject->SetStringField(TEXT("id"), *FString(TEXT("2")));
	JsonObject->SetStringField(TEXT("type"), *FString(TEXT("2")));
	JsonObject->SetStringField(TEXT("content"), *FString(TEXT("2")));

	FString OutputString;

	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&OutputString);

	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);


	TSharedRef<IHttpRequest> Request = Http->CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &AHttpActor::OnResponseReceived);

	//This is the url on which to process the request
	Request->SetURL("http://collection.nupa.moe/api/v1/struct/2");
	Request->SetVerb(HTTPVerb::POST);
	Request->SetContentAsString(OutputString);
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));

	Request->ProcessRequest();
}

void AHttpActor::RequestLogin(const FString& InLoginId)
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &AHttpActor::OnResponseReceived);

	FString RequestURL = CollectionURL + FString("member/") + InLoginId;
	Request->SetURL(RequestURL);
	Request->SetVerb(HTTPVerb::GET);
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));

	Request->ProcessRequest();
}

void AHttpActor::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	//Create a pointer to hold the json serialized data
	TSharedPtr<FJsonObject> JsonObject;

	//Create a reader pointer to read the json data
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	//Deserialize the json data given Reader and the actual object to deserialize
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		////Get the value of the json object by field name
		//int32 recievedInt = JsonObject->GetIntegerField("id");

		////Output it to the engine
		//GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, FString::FromInt(recievedInt));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, Reader.Get().GetErrorMessage());
	}

	OnHttpResponseReceived.ExecuteIfBound(Request, JsonObject, bWasSuccessful);
}

void AHttpActor::BeginPlay()
{
	//HttpCall();

	Super::BeginPlay();
}

void AHttpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

