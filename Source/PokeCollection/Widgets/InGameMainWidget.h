// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameMainWidget.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class POKECOLLECTION_API UInGameMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	//virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

private:
	UFUNCTION()
	void OnStartButtonClicked();

	UFUNCTION()
	void OnPartyMakeButtonClicked();

	UFUNCTION()
	void OnAdventureButtonClicked();

	bool CheckAllButtonsExist();

	/** */
	UPROPERTY(meta = (BindWidget))
	class UButton* TempStartButton = nullptr;
	//

	/** Down Side Buttons */
	UPROPERTY(meta = (BindWidget))
	class UButton* ShopButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* CharacterBoxButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* InventoryButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* SecretBaseButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* WalkButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* MissionButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* DayCareButton = nullptr;
	//

	/** Middle Side Buttons */
	UPROPERTY(meta = (BindWidget))
	class UButton* PartyMakeButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* AdventureButton = nullptr;
	//

};
