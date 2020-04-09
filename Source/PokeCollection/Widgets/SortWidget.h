// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Button.h"
#include "SortWidget.generated.h"

UENUM()
enum class ESortKinds
{
	Level_Ascending,
	Level_Descending,
	Rank_Ascending,
	Rank_Descending,
	Index_Ascending,
	Index_Descending,
	ID_Ascending,	// È¹µæ ¼ø
	ID_Descending,
};

//UENUM()
//enum class ESortContent
//{
//	Character,
//	Equipment,
//};

/** Sort Button Widget */
UCLASS()
class POKECOLLECTION_API USortWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnSortButtonClicked();

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* SortButton;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class USortMenuPopUp> SortMenuPopUpClass;

	UPROPERTY(Transient)
	class USortMenuPopUp* SortMenuPopUp;

};


UCLASS()
class POKECOLLECTION_API USortPopUpButton : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnSelectButtonClicked();

	void SetSortName(FText InSortName);

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* SortSelectButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SortNameText = nullptr;

};


/** Sort Menu Pop Up */
UCLASS(abstract)
class POKECOLLECTION_API USortMenuPopUp : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnBackgroundClicked();
	
private:
	UPROPERTY(meta = (BindWidget))
	class UButton* BackgroundButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* ButtonGridPanel = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TArray<ESortKinds> SortKinds;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class USortPopUpButton> SortPopUpButtonClass;

	UPROPERTY(EditDefaultsOnly)
	int32 ColumnNum = 2;
};
