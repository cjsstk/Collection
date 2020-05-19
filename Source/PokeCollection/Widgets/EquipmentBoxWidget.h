// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/InGameBoxWidget.h"
#include "EquipmentBoxWidget.generated.h"

/** 
 * ĳ���� ��� ���� â�� ������ ��� ���� ����
 */
UCLASS()
class POKECOLLECTION_API UEquipmentSlot : public UBoxSlot
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void InitByID(int32 InContentID) override;

	void SetContentLevel(int32 InLevel);
	bool SetOwnerCharacterImage(int32 InContentID);

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* LevelText = nullptr;
	
	UPROPERTY(meta = (BindWidget))
	class UImage* OwnerCharacterImage = nullptr;

	bool bUseOwnerCharacterImage = false;
};

/**
 * ��� �ڽ��� ������ ��� ����
 */
UCLASS()
class POKECOLLECTION_API UInBoxEquipmentSlot : public UEquipmentSlot
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void OnSelectButtonClicked() override;

protected:

};


UCLASS()
class POKECOLLECTION_API UEquipmentBoxWidget : public UBoxContentWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void OnOpen() override;
	virtual void RefreshSlot() override;

private:

};
