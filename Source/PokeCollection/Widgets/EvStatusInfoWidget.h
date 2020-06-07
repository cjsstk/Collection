// Copyright Me. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/InGameCharacterInfoWidget.h"
#include "PokeCharacter.h"
#include "EvStatusInfoWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAddStatButtonClicked, int32, InStat);

UCLASS()
class POKECOLLECTION_API UEvStatWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void InitSelectedCharacter(APokeCharacter& InCharacter);
	void InitEvStatusInfoWidget(UEvStatusInfoWidget& InInfoWidget);
	void InitEvStat(EStatus InEvStat);

	UFUNCTION()
	void OnAddButtonClicked();

	UFUNCTION()
	void OnMinusButtonClicked();

	//FOnAddStatButtonClicked OnAddStatButtonClicked;

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* EvStatNameText = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* EvStatText = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AddedStatText = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* AddButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* MinusButton = nullptr;

	UPROPERTY(EditDefaultsOnly)
	EStatus EvStat = EStatus::HealthPoint;

	UPROPERTY(Transient)
	APokeCharacter* SelectedCharacter = nullptr;

	UPROPERTY(Transient)
	UEvStatusInfoWidget* InfoWidget = nullptr;

	//int32 StatUpAmount = 0;
};


/**
 * 추가 노력치를 조정 할 수 있는 위젯
 */
UCLASS()
class POKECOLLECTION_API UEvStatusInfoWidget : public UCharacterInfoContentWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void OnOpen() override;

	int32 GetCurrentRemainStatPoint() { return RemainStatPoint; }
	//void SetCurrentRemainStatPoint(int32 InRemainStatPoint) { RemainStatPoint = InRemainStatPoint; }

	int32 GetUpAmount(EStatus InStatus) const;
	void SetUpAmount(EStatus InStatus, int32 InChangeAmount);

	UFUNCTION()
	void OnConfirmButtonClicked();

private:
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* EvStatusBox = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* RemainStatPointText = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* ConfirmButton = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UEvStatWidget> EvStatWidgetClass = nullptr;

	UPROPERTY(Transient)
	TArray<UEvStatWidget*> EvStatWidgets;

	TArray<int32> UpAmount;

	int32 RemainStatPoint = 0;
};
