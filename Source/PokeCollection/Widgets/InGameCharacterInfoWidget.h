// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/InGameMainWidget.h"
#include "TypeChart.h"
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

UCLASS()
class POKECOLLECTION_API UCharacterInfoContentWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void OnOpen() {};

	void SetSelectedCharacter(class APokeCharacter* InSelectedCharacter);

protected:
	class APokeCharacter* GetSelectedCharacter() const { return SelectedCharacter; }

private:
	UPROPERTY(Transient)
	class APokeCharacter* SelectedCharacter = nullptr;

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
 * Character info widget
 */
UCLASS()
class POKECOLLECTION_API UInGameCharacterInfoWidget : public UInGameWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void SetSelectedCharacterID(int32 InSelectedCharacterID) { SelectedCharacterID = InSelectedCharacterID; }
	void SwitchContentWidget(int32 InContentIndex);

private:
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* CatergoryScrollBox = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* InfoContentsBox = nullptr;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FInfoCategoryStruct> InfoContentWidgets;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UInfoCategoryButtonWidget> InfoCategoryButtonClass;

	UPROPERTY(Transient)
	TArray<class UCharacterInfoContentWidget*> InfoContents;

	int32 SelectedCharacterID = 0;

};
