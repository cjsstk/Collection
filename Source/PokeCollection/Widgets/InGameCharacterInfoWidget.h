// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/InGameMainWidget.h"
#include "TypeChart.h"
#include "CMSType.h"
#include "InGameCharacterInfoWidget.generated.h"

UCLASS()
class POKECOLLECTION_API UInfoCategoryButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void SetSwitcherIndex(int32 InIndex) { SwitcherIndex = InIndex; }
	void SetCategoryName(const FText& InCategoryName);

	UFUNCTION()
	void OnCategoryButtonClicked();

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CategoryNameText = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* CategoryButton = nullptr;

	int32 SwitcherIndex = 0;
};

/**
 * 캐릭터 정보 화면에서 표시될 위젯
 */
UCLASS()
class POKECOLLECTION_API UCharacterInfoContentWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void OnOpen();

	void SetSelectedCharacter(class APokeCharacter* InSelectedCharacter);

protected:
	class APokeCharacter* GetSelectedCharacter() const { return CurrentSelectedCharacter; }

private:
	UPROPERTY(Transient)
	class APokeCharacter* CurrentSelectedCharacter = nullptr;

};

USTRUCT()
struct FInfoCategoryStruct
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FText CategoryName;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UCharacterInfoContentWidget> ContentWidgetClass = nullptr;
};

/**
 * 박스에서 들어가는 캐릭터 정보 화면 위젯
 */
UCLASS()
class POKECOLLECTION_API UInGameCharacterInfoWidget : public UInGameWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void OnOpen() override;
	
	virtual void OnBack() override;

	int32 GetSelectedCharacterID() const { return SelectedCharacterID; }
	void SetSelectedCharacterID(int32 InSelectedCharacterID) { SelectedCharacterID = InSelectedCharacterID; }

	void SwitchContentWidget(int32 InContentIndex);

private:
	void SetTypeImages(CharacterType InCharacterType);
	void SetBackgroundColor(ERank InRank);

	UPROPERTY(meta = (BindWidget))
	class UImage* RankBackground = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* CatergoryScrollBox = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* InfoContentsBox = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UImage* CharacterImage = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CharacterName = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UImage* Type1Image = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UImage* Type2Image = nullptr;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FInfoCategoryStruct> InfoContentWidgets;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UInfoCategoryButtonWidget> InfoCategoryButtonClass;

	UPROPERTY(Transient)
	TArray<class UCharacterInfoContentWidget*> InfoContents;

	int32 SelectedCharacterID = -1;

	int32 SelectedContentWidgetIndex = 0;

};
