// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PokeCore.h"
#include "PokeSortInfo.h"
#include "SortWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSortPopUpButtonClicked, FPokeSortInfo, InSortInfo);


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
	void SetSortInfo(FPokeSortInfo InSortInfo);
	
	FOnSortPopUpButtonClicked OnSortPopUpButtonClicked;

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* SortSelectButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SortNameText = nullptr;

	FPokeSortInfo SortInfo;

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
	TArray<FPokeSortInfo> SortInfos;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class USortPopUpButton> SortPopUpButtonClass;

	UPROPERTY(EditDefaultsOnly)
	int32 ColumnNum = 2;
};
