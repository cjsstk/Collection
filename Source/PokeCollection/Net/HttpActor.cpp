// Copyright Me. All Rights Reserved.


#include "HttpActor.h"

#include "Engine.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

#include "PokeCore.h"
#include "PokeCollectionCharacter.h"
#include "PokeCharacter.h"
#include "PokeEquipment.h"
#include "PokeItem.h"

const static FString CollectionURL = FString("http://collection.nupa.moe/api/");

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

	FString LoadedToken;
	FString FileName = FPaths::ProjectConfigDir() + "/Token.txt";
	if (FFileHelper::LoadFileToString(LoadedToken, *FileName))
	{
		AuthToken = LoadedToken;
	}
	else
	{
		ensure(0);
	}
}

void AHttpActor::Request(const FHttpRequestParams& InRequestParams)
{
	Requests.Enqueue(InRequestParams);
}

void AHttpActor::RequestLogin(const FString& InLoginId)
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &AHttpActor::HttpResponseReceived);

	FString RequestURL = CollectionURL + FString("members/") + InLoginId;
	Request->SetURL(RequestURL);
	Request->SetVerb(HTTPVerb::GET);
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->SetHeader("Authorization", AuthToken);
	Request->SetHeader("X-Request-ID", FString::FromInt((int32)EHttpRequestType::Login));

	Request->ProcessRequest();
}

void AHttpActor::RequestRegist(const FString& InRegistId)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	JsonObject->SetStringField(TEXT("id"), *FString::Printf(TEXT("%s"), *InRegistId));
	JsonObject->SetStringField(TEXT("nickname"), *FString(TEXT("Temp")));
	JsonObject->SetNumberField(TEXT("level"), 1);
	JsonObject->SetNumberField(TEXT("exp"), 0);
	JsonObject->SetNumberField(TEXT("money"), 0);
	JsonObject->SetNumberField(TEXT("berry"), 0);
	JsonObject->SetNumberField(TEXT("stardust"), 0);
	JsonObject->SetNumberField(TEXT("maxClearBattleStage"), 0);
	JsonObject->SetNumberField(TEXT("lastSelectedChapter"), 1);
	JsonObject->SetNumberField(TEXT("maxOpenedChapter"), 1);
	JsonObject->SetNumberField(TEXT("lastSelectedParty"), 1);
	JsonObject->SetNumberField(TEXT("battleClearCount"), 0);
	JsonObject->SetNumberField(TEXT("getCharacterCount"), 0);
	JsonObject->SetNumberField(TEXT("maxHaveCharactersNum"), 100);
	JsonObject->SetNumberField(TEXT("maxHaveEquipmentsNum"), 100);
	JsonObject->SetStringField(TEXT("index"), *FString(TEXT("")));
	JsonObject->SetNumberField(TEXT("battleSpeed"), 1);

	FString OutputString;

	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&OutputString);

	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &AHttpActor::HttpResponseReceived);

	FString RequestURL = CollectionURL + FString("members/") + InRegistId;
	Request->SetURL(RequestURL);
	Request->SetVerb(HTTPVerb::POST);
	Request->SetContentAsString(OutputString);
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->SetHeader("Authorization", AuthToken);
	Request->SetHeader("X-Request-ID", FString::FromInt((int32)EHttpRequestType::Regist));

	Request->ProcessRequest();
}

void AHttpActor::RequestHaveCharacters(const FString& InUserId)
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &AHttpActor::HttpResponseReceived);

	FString RequestURL = CollectionURL + FString("members/") + InUserId + FString("/haveCharacters");
	Request->SetURL(RequestURL);
	Request->SetVerb(HTTPVerb::GET);
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->SetHeader("Authorization", AuthToken);
	Request->SetHeader("X-Request-ID", FString::FromInt((int32)EHttpRequestType::HaveCharacters));

	Request->ProcessRequest();
}

void AHttpActor::RequestHaveEquipments(const FString& InUserId)
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &AHttpActor::HttpResponseReceived);

	FString RequestURL = CollectionURL + FString("members/") + InUserId + FString("/haveEquipments");
	Request->SetURL(RequestURL);
	Request->SetVerb(HTTPVerb::GET);
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->SetHeader("Authorization", AuthToken);
	Request->SetHeader("X-Request-ID", FString::FromInt((int32)EHttpRequestType::HaveEquipments));

	Request->ProcessRequest();
}

void AHttpActor::RequestHaveItems(const FString& InUserId)
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &AHttpActor::HttpResponseReceived);

	FString RequestURL = CollectionURL + FString("members/") + InUserId + FString("/haveItems");
	Request->SetURL(RequestURL);
	Request->SetVerb(HTTPVerb::GET);
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->SetHeader("Authorization", AuthToken);
	Request->SetHeader("X-Request-ID", FString::FromInt((int32)EHttpRequestType::HaveItems));

	Request->ProcessRequest();
}

void AHttpActor::RequestAddNewCharacters(const FString& InUserId, const TArray<FInitCharacterParams>& NewCharactersInfos)
{
	TArray<TSharedPtr<FJsonValue>> ObjArray;

	for (auto&& InitParam : NewCharactersInfos)
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

		JsonObject->SetStringField(TEXT("memberId"), *FString::Printf(TEXT("%s"), *InUserId));
		JsonObject->SetNumberField(TEXT("characterId"), InitParam.CharacterID);
		JsonObject->SetNumberField(TEXT("characterKey"), InitParam.CharacterKey);
		JsonObject->SetNumberField(TEXT("characterLevel"), InitParam.CharacterLevel);
		JsonObject->SetNumberField(TEXT("characterExp"), InitParam.CurrentExp);
		JsonObject->SetNumberField(TEXT("joinedParty"), InitParam.JoinedPartyNum);
		JsonObject->SetNumberField(TEXT("joinedSlot"), InitParam.JoinedSlotNum);
		JsonObject->SetNumberField(TEXT("EvHealth"), InitParam.EvHealth);
		JsonObject->SetNumberField(TEXT("EvAttack"), InitParam.EvAttack);
		JsonObject->SetNumberField(TEXT("EvDefense"), InitParam.EvDefence);
		JsonObject->SetNumberField(TEXT("EvSpAttack"), InitParam.EvSpecialAttack);
		JsonObject->SetNumberField(TEXT("EvSpDefense"), InitParam.EvSpecialDefence);
		JsonObject->SetNumberField(TEXT("EvSpeed"), InitParam.EvSpeed);
		JsonObject->SetNumberField(TEXT("skill1Level"), InitParam.Skill1Level);
		JsonObject->SetNumberField(TEXT("skill2Level"), InitParam.Skill2Level);
		JsonObject->SetNumberField(TEXT("skill3Level"), InitParam.Skill3Level);
		JsonObject->SetNumberField(TEXT("skill4Level"), InitParam.Skill4Level);

		TSharedRef<FJsonValueObject> JsonValue = MakeShareable(new FJsonValueObject(JsonObject));
		ObjArray.Add(JsonValue);
	}

	TSharedPtr<FJsonObject> SendJsonObject = MakeShareable(new FJsonObject());
	SendJsonObject->SetArrayField("characters", ObjArray);

	FString OutputString;

	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&OutputString);

	FJsonSerializer::Serialize(SendJsonObject.ToSharedRef(), JsonWriter);

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &AHttpActor::HttpResponseReceived);

	FString RequestURL = CollectionURL + FString("members/") + InUserId + FString("/haveCharacters");
	Request->SetURL(RequestURL);
	Request->SetVerb(HTTPVerb::POST);
	Request->SetContentAsString(OutputString);
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->SetHeader("Authorization", AuthToken);
	Request->SetHeader("X-Request-ID", FString::FromInt((int32)EHttpRequestType::AddNewCharacters));

	Request->ProcessRequest();
}

void AHttpActor::RequestAddNewEquipments(const FString& InUserId, const TArray<FInitEquipmentParams>& NewEquipmentsInfos)
{
	TArray<TSharedPtr<FJsonValue>> ObjArray;

	for (auto&& InitParam : NewEquipmentsInfos)
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

		JsonObject->SetStringField(TEXT("memberId"), *FString::Printf(TEXT("%s"), *InUserId));
		JsonObject->SetNumberField(TEXT("equipmentId"), InitParam.EquipmentID);
		JsonObject->SetNumberField(TEXT("equipmentKey"), InitParam.EquipmentKey);
		JsonObject->SetNumberField(TEXT("equipmentLevel"), InitParam.EquipmentLevel);
		JsonObject->SetNumberField(TEXT("ownerCharacterId"), InitParam.OwnerCharacterID);

		TSharedRef<FJsonValueObject> JsonValue = MakeShareable(new FJsonValueObject(JsonObject));
		ObjArray.Add(JsonValue);
	}

	TSharedPtr<FJsonObject> SendJsonObject = MakeShareable(new FJsonObject());
	SendJsonObject->SetArrayField("equipments", ObjArray);

	FString OutputString;

	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&OutputString);

	FJsonSerializer::Serialize(SendJsonObject.ToSharedRef(), JsonWriter);

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &AHttpActor::HttpResponseReceived);

	FString RequestURL = CollectionURL + FString("members/") + InUserId + FString("/haveEquipments");
	Request->SetURL(RequestURL);
	Request->SetVerb(HTTPVerb::POST);
	Request->SetContentAsString(OutputString);
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->SetHeader("Authorization", AuthToken);
	Request->SetHeader("X-Request-ID", FString::FromInt((int32)EHttpRequestType::AddNewEquipments));

	Request->ProcessRequest();
}

void AHttpActor::RequestAddNewItems(const FString& InUserId, const TArray<FInitItemParams>& NewItemsInfos)
{
	TArray<TSharedPtr<FJsonValue>> ObjArray;

	for (auto&& InitParam : NewItemsInfos)
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

		JsonObject->SetNumberField(TEXT("itemId"), InitParam.ItemID);
		JsonObject->SetNumberField(TEXT("itemKey"), InitParam.ItemKey);
		JsonObject->SetNumberField(TEXT("stackNum"), InitParam.ItemStackNum);
		
		TSharedRef<FJsonValueObject> JsonValue = MakeShareable(new FJsonValueObject(JsonObject));
		ObjArray.Add(JsonValue);
	}

	TSharedPtr<FJsonObject> SendJsonObject = MakeShareable(new FJsonObject());
	SendJsonObject->SetArrayField("items", ObjArray);

	FString OutputString;

	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&OutputString);

	FJsonSerializer::Serialize(SendJsonObject.ToSharedRef(), JsonWriter);

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &AHttpActor::HttpResponseReceived);

	FString RequestURL = CollectionURL + FString("members/") + InUserId + FString("/haveItems");
	Request->SetURL(RequestURL);
	Request->SetVerb(HTTPVerb::POST);
	Request->SetContentAsString(OutputString);
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->SetHeader("Authorization", AuthToken);
	Request->SetHeader("X-Request-ID", FString::FromInt((int32)EHttpRequestType::AddNewItems));

	Request->ProcessRequest();
}

void AHttpActor::RequestDestroyCharacters(const FString& InUserId, const TArray<int32>& DestroyCharacterIds)
{
	TArray<TSharedPtr<FJsonValue>> ObjArray;

	for (auto&& CharacterId : DestroyCharacterIds)
	{
		TSharedRef<FJsonValueNumber> JsonValue = MakeShareable(new FJsonValueNumber(CharacterId));
		ObjArray.Add(JsonValue);
	}

	TSharedPtr<FJsonObject> SendJsonObject = MakeShareable(new FJsonObject());
	SendJsonObject->SetArrayField("characterIds", ObjArray);

	FString OutputString;

	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&OutputString);

	FJsonSerializer::Serialize(SendJsonObject.ToSharedRef(), JsonWriter);

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &AHttpActor::HttpResponseReceived);

	FString RequestURL = CollectionURL + FString("members/") + InUserId + FString("/haveCharacters");
	Request->SetURL(RequestURL);
	Request->SetVerb(HTTPVerb::DELETE);
	Request->SetContentAsString(OutputString);
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->SetHeader("Authorization", AuthToken);
	Request->SetHeader("X-Request-ID", FString::FromInt((int32)EHttpRequestType::DestroyCharacters));

	Request->ProcessRequest();
}

void AHttpActor::RequestDestroyEquipments(const FString& InUserId, const TArray<int32>& DestroyEquipmentIds)
{
	TArray<TSharedPtr<FJsonValue>> ObjArray;

	for (auto&& EquipmentId : DestroyEquipmentIds)
	{
		TSharedRef<FJsonValueNumber> JsonValue = MakeShareable(new FJsonValueNumber(EquipmentId));
		ObjArray.Add(JsonValue);
	}

	TSharedPtr<FJsonObject> SendJsonObject = MakeShareable(new FJsonObject());
	SendJsonObject->SetArrayField("equipmentIds", ObjArray);

	FString OutputString;

	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&OutputString);

	FJsonSerializer::Serialize(SendJsonObject.ToSharedRef(), JsonWriter);

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &AHttpActor::HttpResponseReceived);

	FString RequestURL = CollectionURL + FString("members/") + InUserId + FString("/haveEquipments");
	Request->SetURL(RequestURL);
	Request->SetVerb(HTTPVerb::DELETE);
	Request->SetContentAsString(OutputString);
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->SetHeader("Authorization", AuthToken);
	Request->SetHeader("X-Request-ID", FString::FromInt((int32)EHttpRequestType::DestroyEquipments));

	Request->ProcessRequest();
}

void AHttpActor::RequestDestroyItems(const FString& InUserId, const TMap<int32, int32>& DestroyItemKeys)
{
	APokeCollectionCharacter* Player = PokeCore::GetPokePlayer(GetWorld());
	if (!Player)
	{
		ensure(0);
		return;
	}

	TArray<TSharedPtr<FJsonValue>> ObjArray;

	for (auto&& Item : DestroyItemKeys)
	{
		UPokeItem* PokeItem = Player->GetItemByKey(Item.Key);
		if (!PokeItem)
		{
			continue;
		}

		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

		JsonObject->SetNumberField(TEXT("itemId"), PokeItem->GetItemID());
		JsonObject->SetNumberField(TEXT("itemKey"), Item.Key);
		JsonObject->SetNumberField(TEXT("stackNum"), Item.Value);

		TSharedRef<FJsonValueObject> JsonValue = MakeShareable(new FJsonValueObject(JsonObject));
		ObjArray.Add(JsonValue);
	}

	TSharedPtr<FJsonObject> SendJsonObject = MakeShareable(new FJsonObject());
	SendJsonObject->SetArrayField("items", ObjArray);

	FString OutputString;

	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&OutputString);

	FJsonSerializer::Serialize(SendJsonObject.ToSharedRef(), JsonWriter);

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &AHttpActor::HttpResponseReceived);

	FString RequestURL = CollectionURL + FString("members/") + InUserId + FString("/haveItems");
	Request->SetURL(RequestURL);
	Request->SetVerb(HTTPVerb::DELETE);
	Request->SetContentAsString(OutputString);
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->SetHeader("Authorization", AuthToken);
	Request->SetHeader("X-Request-ID", FString::FromInt((int32)EHttpRequestType::DestroyItems));

	Request->ProcessRequest();
}

void AHttpActor::RequestUpdateCharacters(const FString& InUserId, const TArray<int32>& UpdateCharacterIds)
{
	APokeCollectionCharacter* Player = PokeCore::GetPokePlayer(GetWorld());
	if (!Player)
	{
		ensure(0);
		return;
	}

	TArray<TSharedPtr<FJsonValue>> ObjArray;

	for (auto&& UpdateCharacterId : UpdateCharacterIds)
	{
		APokeCharacter* PokeCharacter = Player->GetCharacterByID(UpdateCharacterId);
		if (!PokeCharacter)
		{
			continue;
		}

		TArray<int32> SkillLevels;
		PokeCharacter->GetSkillLevels(SkillLevels);

		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

		JsonObject->SetStringField(TEXT("memberId"), *FString::Printf(TEXT("%s"), *InUserId));
		JsonObject->SetNumberField(TEXT("characterId"), UpdateCharacterId);
		JsonObject->SetNumberField(TEXT("characterKey"), PokeCharacter->GetCharacterKey());
		JsonObject->SetNumberField(TEXT("characterLevel"), PokeCharacter->GetLevel());
		JsonObject->SetNumberField(TEXT("characterExp"), PokeCharacter->GetCurrentExp());
		JsonObject->SetNumberField(TEXT("joinedParty"), PokeCharacter->GetJoinedPartyNum());
		JsonObject->SetNumberField(TEXT("joinedSlot"), PokeCharacter->GetJoinedSlotNum());
		JsonObject->SetNumberField(TEXT("EvHealth"), PokeCharacter->GetEvStat(EStatus::HealthPoint));
		JsonObject->SetNumberField(TEXT("EvAttack"), PokeCharacter->GetEvStat(EStatus::Attack));
		JsonObject->SetNumberField(TEXT("EvDefense"), PokeCharacter->GetEvStat(EStatus::Defense));
		JsonObject->SetNumberField(TEXT("EvSpAttack"), PokeCharacter->GetEvStat(EStatus::SpAttack));
		JsonObject->SetNumberField(TEXT("EvSpDefense"), PokeCharacter->GetEvStat(EStatus::SpDefense));
		JsonObject->SetNumberField(TEXT("EvSpeed"), PokeCharacter->GetEvStat(EStatus::Speed));
		JsonObject->SetNumberField(TEXT("skill1Level"), SkillLevels[0]);
		JsonObject->SetNumberField(TEXT("skill2Level"), SkillLevels[1]);
		JsonObject->SetNumberField(TEXT("skill3Level"), SkillLevels[2]);
		JsonObject->SetNumberField(TEXT("skill4Level"), SkillLevels[3]);

		TSharedRef<FJsonValueObject> JsonValue = MakeShareable(new FJsonValueObject(JsonObject));
		ObjArray.Add(JsonValue);
	}

	TSharedPtr<FJsonObject> SendJsonObject = MakeShareable(new FJsonObject());
	SendJsonObject->SetArrayField("characters", ObjArray);

	FString OutputString;

	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&OutputString);

	FJsonSerializer::Serialize(SendJsonObject.ToSharedRef(), JsonWriter);

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &AHttpActor::HttpResponseReceived);

	FString RequestURL = CollectionURL + FString("members/") + InUserId + FString("/haveCharacters");
	Request->SetURL(RequestURL);
	Request->SetVerb(HTTPVerb::PUT);
	Request->SetContentAsString(OutputString);
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->SetHeader("Authorization", AuthToken);
	Request->SetHeader("X-Request-ID", FString::FromInt((int32)EHttpRequestType::UpdateCharacters));

	Request->ProcessRequest();
}

void AHttpActor::RequestUpdateEquipments(const FString& InUserId, const TArray<int32>& UpdateEquipmentIds)
{
	APokeCollectionCharacter* Player = PokeCore::GetPokePlayer(GetWorld());
	if (!Player)
	{
		ensure(0);
		return;
	}

	TArray<TSharedPtr<FJsonValue>> ObjArray;

	for (auto&& UpdateEquipmentId : UpdateEquipmentIds)
	{
		UPokeEquipment* PokeEquipment = Player->GetEquipmentByID(UpdateEquipmentId);
		if (!PokeEquipment)
		{
			continue;
		}

		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

		JsonObject->SetStringField(TEXT("memberId"), *FString::Printf(TEXT("%s"), *InUserId));
		JsonObject->SetNumberField(TEXT("equipmentId"), UpdateEquipmentId);
		JsonObject->SetNumberField(TEXT("equipmentKey"), PokeEquipment->GetEquipmentKey());
		JsonObject->SetNumberField(TEXT("equipmentLevel"), PokeEquipment->GetLevel());
		JsonObject->SetNumberField(TEXT("ownerCharacterId"), PokeEquipment->GetOwnerCharacterID());

		TSharedRef<FJsonValueObject> JsonValue = MakeShareable(new FJsonValueObject(JsonObject));
		ObjArray.Add(JsonValue);
	}

	TSharedPtr<FJsonObject> SendJsonObject = MakeShareable(new FJsonObject());
	SendJsonObject->SetArrayField("equipments", ObjArray);

	FString OutputString;

	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&OutputString);

	FJsonSerializer::Serialize(SendJsonObject.ToSharedRef(), JsonWriter);

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &AHttpActor::HttpResponseReceived);

	FString RequestURL = CollectionURL + FString("members/") + InUserId + FString("/haveEquipments");
	Request->SetURL(RequestURL);
	Request->SetVerb(HTTPVerb::PUT);
	Request->SetContentAsString(OutputString);
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->SetHeader("Authorization", AuthToken);
	Request->SetHeader("X-Request-ID", FString::FromInt((int32)EHttpRequestType::UpdateEquipments));

	Request->ProcessRequest();
}

void AHttpActor::RequestSavePlayerInfo(const FString& InUserId, const ESavePlayerInfo& InColumnName, const FInitPlayerParams& Params)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	JsonObject->SetStringField(TEXT("id"), *FString::Printf(TEXT("%s"), *InUserId));

	switch (InColumnName)
	{
	case ESavePlayerInfo::Nickname:
		JsonObject->SetStringField(TEXT("nickname"), *Params.PlayerNickname);
		break;
	case ESavePlayerInfo::Level:
		JsonObject->SetNumberField(TEXT("level"), Params.PlayerLevel);
		break;
	case ESavePlayerInfo::Exp:
		JsonObject->SetNumberField(TEXT("exp"), Params.PlayerCurrentExp);
		break;
	case ESavePlayerInfo::Money:
		JsonObject->SetNumberField(TEXT("money"), Params.PlayerMoney);
		break;
	case ESavePlayerInfo::Berry:
		JsonObject->SetNumberField(TEXT("berry"), Params.PlayerBerry);
		break;
	case ESavePlayerInfo::Stardust:
		JsonObject->SetNumberField(TEXT("stardust"), Params.PlayerStardust);
		break;
	case ESavePlayerInfo::MaxClearBattleStage:
		JsonObject->SetNumberField(TEXT("maxClearBattleStage"), Params.MaxClearBattleStage);
		break;
	case ESavePlayerInfo::LastSelectedChapter:
		JsonObject->SetNumberField(TEXT("lastSelectedChapter"), Params.LastSelectedChapter);
		break;
	case ESavePlayerInfo::MaxOpenedChapter:
		JsonObject->SetNumberField(TEXT("maxOpenedChapter"), Params.MaxOpenedChapter);
		break;
	case ESavePlayerInfo::LastSelectedParty:
		JsonObject->SetNumberField(TEXT("lastSelectedParty"), Params.LastSelectedParty);
		break;
	case ESavePlayerInfo::BattleClearCount:
		JsonObject->SetNumberField(TEXT("battleClearCount"), Params.BattleClearCount);
		break;
	case ESavePlayerInfo::GetCharacterCount:
		JsonObject->SetNumberField(TEXT("getCharacterCount"), Params.GetCharacterCount);
		break;
	case ESavePlayerInfo::MaxHaveCharactersNum:
		JsonObject->SetNumberField(TEXT("maxHaveCharactersNum"), Params.MaxHaveCharactersNum);
		break;
	case ESavePlayerInfo::MaxHaveEquipmentsNum:
		JsonObject->SetNumberField(TEXT("maxHaveEquipmentsNum"), Params.MaxHaveEquipmentsNum);
		break;
	case ESavePlayerInfo::Index:
		JsonObject->SetStringField(TEXT("index"), *Params.Index);
		break;
	case ESavePlayerInfo::BattleSpeed:
		JsonObject->SetNumberField(TEXT("battleSpeed"), Params.BattleSpeed);
		break;
	default:
		break;
	}
	
	FString OutputString;

	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&OutputString);

	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &AHttpActor::HttpResponseReceived);

	FString RequestURL = CollectionURL + FString("members/") + InUserId;
	Request->SetURL(RequestURL);
	Request->SetVerb(HTTPVerb::PUT);
	Request->SetContentAsString(OutputString);
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->SetHeader("Authorization", AuthToken);
	Request->SetHeader("X-Request-ID", FString::FromInt((int32)EHttpRequestType::SavePlayerInfo));

	Request->ProcessRequest();
}

void AHttpActor::OnLoginResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
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

	OnHttpLoginResponseReceived.ExecuteIfBound(Request, JsonObject, bWasSuccessful);
}

void AHttpActor::OnRegistResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	TSharedPtr<FJsonObject> JsonObject;

	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		OnHttpRegistResponseReceived.ExecuteIfBound(Request, JsonObject, bWasSuccessful);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, Reader.Get().GetErrorMessage());
	}

	OnHttpRegistResponseReceived.ExecuteIfBound(Request, JsonObject, bWasSuccessful);
}

void AHttpActor::OnHaveCharactersResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	TSharedPtr<FJsonObject> JsonObject;

	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		OnHttpHaveCharactersResponseReceived.ExecuteIfBound(Request, JsonObject, bWasSuccessful);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, Reader.Get().GetErrorMessage());
	}
}

void AHttpActor::OnHaveEquipmentsResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	TSharedPtr<FJsonObject> JsonObject;

	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		OnHttpHaveEquipmentsResponseReceived.ExecuteIfBound(Request, JsonObject, bWasSuccessful);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, Reader.Get().GetErrorMessage());
	}
}

void AHttpActor::OnHaveItemsResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	TSharedPtr<FJsonObject> JsonObject;

	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		OnHttpHaveItemsResponseReceived.ExecuteIfBound(Request, JsonObject, bWasSuccessful);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, Reader.Get().GetErrorMessage());
	}
}

void AHttpActor::OnSavePlayerInfoResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	TSharedPtr<FJsonObject> JsonObject;

	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Red, FString(TEXT("유저 데이터 저장")));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, Reader.Get().GetErrorMessage());
	}
}

void AHttpActor::OnAddNewCharactersResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	TSharedPtr<FJsonObject> JsonObject;

	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Red, FString(TEXT("새 캐릭터 추가")));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, Reader.Get().GetErrorMessage());
	}
}

void AHttpActor::OnAddNewEquipmentsResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	TSharedPtr<FJsonObject> JsonObject;

	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Red, FString(TEXT("새 장비 추가")));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, Reader.Get().GetErrorMessage());
	}
}

void AHttpActor::OnAddNewItemsResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	TSharedPtr<FJsonObject> JsonObject;

	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Red, FString(TEXT("새 아이템 추가")));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, Reader.Get().GetErrorMessage());
	}
}

void AHttpActor::OnDestroyCharactersResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	TSharedPtr<FJsonObject> JsonObject;

	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Red, FString(TEXT("캐릭터 삭제")));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, Reader.Get().GetErrorMessage());
	}
}

void AHttpActor::OnDestroyEquipmentsResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	TSharedPtr<FJsonObject> JsonObject;

	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Red, FString(TEXT("장비 삭제")));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, Reader.Get().GetErrorMessage());
	}
}

void AHttpActor::OnDestroyItemsResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	TSharedPtr<FJsonObject> JsonObject;

	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Red, FString(TEXT("아이템 삭제")));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, Reader.Get().GetErrorMessage());
	}
}

void AHttpActor::OnUpdateCharactersResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	TSharedPtr<FJsonObject> JsonObject;

	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Red, FString(TEXT("캐릭터 수정")));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, Reader.Get().GetErrorMessage());
	}
}

void AHttpActor::OnUpdateEquipmentsResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	TSharedPtr<FJsonObject> JsonObject;

	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Red, FString(TEXT("장비 수정")));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, Reader.Get().GetErrorMessage());
	}
}

void AHttpActor::BeginPlay()
{
	Super::BeginPlay();
}

void AHttpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bRequesting)
	{
		if (Requests.Peek())
		{
			const FHttpRequestParams& RequestParams = *Requests.Peek();

			if (RequestParams.RequestType == EHttpRequestType::Invalid)
			{
				ensure(0);
				Requests.Pop();
				return;
			}

			HttpRequest(RequestParams);
		}
	}
}

void AHttpActor::HttpRequest(const FHttpRequestParams& InRequestParams)
{
	bRequesting = true;

	switch (InRequestParams.RequestType)
	{
	case EHttpRequestType::Login:
		RequestLogin(InRequestParams.MemberID);
		break;
	case EHttpRequestType::Regist:
		RequestRegist(InRequestParams.MemberID);
		break;
	case EHttpRequestType::HaveCharacters:
		RequestHaveCharacters(InRequestParams.MemberID);
		break;
	case EHttpRequestType::HaveEquipments:
		RequestHaveEquipments(InRequestParams.MemberID);
		break;
	case EHttpRequestType::HaveItems:
		RequestHaveItems(InRequestParams.MemberID);
		break;
	case EHttpRequestType::SavePlayerInfo:
		RequestSavePlayerInfo(InRequestParams.MemberID, InRequestParams.SaveColumn, InRequestParams.SavePlayerInfos);
		break;
	case EHttpRequestType::AddNewCharacters:
		RequestAddNewCharacters(InRequestParams.MemberID, InRequestParams.NewCharactersInfos);
		break;
	case EHttpRequestType::AddNewEquipments:
		RequestAddNewEquipments(InRequestParams.MemberID, InRequestParams.NewEquipmentsInfos);
		break;
	case EHttpRequestType::AddNewItems:
		RequestAddNewItems(InRequestParams.MemberID, InRequestParams.NewItemsInfos);
		break;
	case EHttpRequestType::DestroyCharacters:
		RequestDestroyCharacters(InRequestParams.MemberID, InRequestParams.DestoryCharacterIds);
		break;
	case EHttpRequestType::DestroyEquipments:
		RequestDestroyEquipments(InRequestParams.MemberID, InRequestParams.DestoryEquipmentIds);
		break;
	case EHttpRequestType::DestroyItems:
		RequestDestroyItems(InRequestParams.MemberID, InRequestParams.DestoryItemKeys);
		break;
	case EHttpRequestType::UpdateCharacters:
		RequestUpdateCharacters(InRequestParams.MemberID, InRequestParams.UpdateCharacterIds);
		break;
	case EHttpRequestType::UpdateEquipments:
		RequestUpdateEquipments(InRequestParams.MemberID, InRequestParams.UpdateEquipmentIds);
		break;
	case EHttpRequestType::Invalid:
	default:
		bRequesting = false;
		break;
	}
}

void AHttpActor::HttpResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	bRequesting = false;
	Requests.Pop();

	FString RequestTypeStr = Request->GetHeader("X-Request-ID");

	EHttpRequestType RequestType = (EHttpRequestType)FCString::Atoi(*RequestTypeStr);

	if (RequestType == EHttpRequestType::Invalid)
	{
		ensure(0);
		return;
	}

	switch (RequestType)
	{
	case EHttpRequestType::Login:
		OnLoginResponseReceived(Request, Response, bWasSuccessful);
		break;
	case EHttpRequestType::Regist:
		OnRegistResponseReceived(Request, Response, bWasSuccessful);
		break;
	case EHttpRequestType::HaveCharacters:
		OnHaveCharactersResponseReceived(Request, Response, bWasSuccessful);
		break;
	case EHttpRequestType::HaveEquipments:
		OnHaveEquipmentsResponseReceived(Request, Response, bWasSuccessful);
		break;
	case EHttpRequestType::HaveItems:
		OnHaveItemsResponseReceived(Request, Response, bWasSuccessful);
		break;
	case EHttpRequestType::SavePlayerInfo:
		OnSavePlayerInfoResponseReceived(Request, Response, bWasSuccessful);
		break;
	case EHttpRequestType::AddNewCharacters:
		OnAddNewCharactersResponseReceived(Request, Response, bWasSuccessful);
		break;
	case EHttpRequestType::AddNewEquipments:
		OnAddNewEquipmentsResponseReceived(Request, Response, bWasSuccessful);
		break;
	case EHttpRequestType::AddNewItems:
		OnAddNewItemsResponseReceived(Request, Response, bWasSuccessful);
		break;
	case EHttpRequestType::DestroyCharacters:
		OnDestroyCharactersResponseReceived(Request, Response, bWasSuccessful);
		break;
	case EHttpRequestType::DestroyEquipments:
		OnDestroyEquipmentsResponseReceived(Request, Response, bWasSuccessful);
		break;
	case EHttpRequestType::DestroyItems:
		OnDestroyItemsResponseReceived(Request, Response, bWasSuccessful);
		break;
	case EHttpRequestType::UpdateCharacters:
		OnUpdateCharactersResponseReceived(Request, Response, bWasSuccessful);
		break;
	case EHttpRequestType::UpdateEquipments:
		OnUpdateEquipmentsResponseReceived(Request, Response, bWasSuccessful);
		break;
	case EHttpRequestType::Invalid:
	default:
		ensure(0);
		break;
	}

}

