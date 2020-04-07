// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/InGameMainWidget.h"
#include "CMSType.h"
#include "InGameBoxWidget.generated.h"


UENUM()
enum class EBoxContentType
{
	Character,
	Equipment,
};

UCLASS(abstract)
class POKECOLLECTION_API UBoxSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnSelectContentButtonClicked();

	void SetContentImage(UTexture2D* InContentTexture);
	void SetContentID(EBoxContentType InBoxContentType, int32 InContentID);
	void SetContentName(const FText& InName);
	void SetContentLevel(int32 InLevel);
	void SetBackgroundColor(ERank InRank);

protected:
	UPROPERTY(meta = (BindWidget))
	class UImage* BackgroundImage = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UImage* ProfileImage = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* SelectContentButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* NameText = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* LevelText = nullptr;

	int32 ContentID = 0;

	EBoxContentType BoxContentType = EBoxContentType::Character;
	
};


UCLASS(abstract)
class POKECOLLECTION_API UBoxContentWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void OnOpen() {};

protected:
	UPROPERTY(meta = (BindWidget))
	class UImage* BackgroundImage = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ScrollBox = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* GridPanel = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UBoxSlot> BoxSlotClass;

	UPROPERTY(EditDefaultsOnly)
	EBoxContentType BoxContentType = EBoxContentType::Character;

	UPROPERTY(EditDefaultsOnly)
	int32 ColumnNum = 7;

};


USTRUCT()
struct FBoxContentStruct
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FText ContentName;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UBoxContentWidget> ContentWidgetClass = nullptr;
};

UCLASS()
class POKECOLLECTION_API UInGameBoxWidget : public UInGameWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void OnOpen() override;

	UFUNCTION()
	void OnCharacterBoxButtonClicked();
	
	UFUNCTION()
	void OnEquipmentBoxButtonClicked();


private:
	UPROPERTY(meta = (BindWidget))
	class UButton* CharacterBoxButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* EquipmentBoxButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* ContentsBox = nullptr;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FBoxContentStruct> ContentWidgets;

	UPROPERTY(Transient)
	TArray<class UBoxContentWidget*> BoxContents;
};
