// Copyright Me. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/BasicPopUp.h"
#include "DestroyConfirmPopUp.generated.h"

/**
 * 캐릭터 분해 확인 팝업
 */
UCLASS()
class POKECOLLECTION_API UDestroyCharacterConfirmPopUp : public UBasicPopUp
{
	GENERATED_BODY()
	
public:
	virtual void OnConfirmed() override;
};


/**
 * 장비 분해 확인 팝업
 */
UCLASS()
class POKECOLLECTION_API UDestroyEquipmentConfirmPopUp : public UBasicPopUp
{
	GENERATED_BODY()

};