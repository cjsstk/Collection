// Copyright Me. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MakePartySlotsStatusInfo.generated.h"

/**
 * ��Ƽ �� â�� ��Ÿ���� �ɷ�ġ ���� ���� ����
 */
UCLASS()
class POKECOLLECTION_API UMakePartySlotsStatusInfo : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Refresh();

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TotalHealthText = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TotalAttackText = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TotalDefenseText = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TotalSpAttackText = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TotalSpDefenseText = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TotalSpeedText = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TotalConsumeText = nullptr;
};
