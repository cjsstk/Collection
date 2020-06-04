// Copyright Me. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/BasicPopUp.h"
#include "DestroyConfirmPopUp.generated.h"

/**
 * ĳ���� ���� Ȯ�� �˾�
 */
UCLASS()
class POKECOLLECTION_API UDestroyCharacterConfirmPopUp : public UBasicPopUp
{
	GENERATED_BODY()
	
public:
	virtual void OnConfirmed() override;
};


/**
 * ��� ���� Ȯ�� �˾�
 */
UCLASS()
class POKECOLLECTION_API UDestroyEquipmentConfirmPopUp : public UBasicPopUp
{
	GENERATED_BODY()

};