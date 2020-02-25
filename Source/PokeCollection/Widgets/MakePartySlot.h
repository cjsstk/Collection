// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MakePartySlot.generated.h"

UCLASS()
class POKECOLLECTION_API UMakePartySlot : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	int32 GetSlotNum() const { return SlotNum; }

private:
	UFUNCTION()
	void OnChangeCharacterButtonClicked();

	UPROPERTY(meta = (BindWidget))
	class UButton* ChangeCharacterButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 SlotNum = 0;

};
