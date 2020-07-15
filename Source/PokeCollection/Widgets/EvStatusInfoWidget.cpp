// Copyright Me. All Rights Reserved.


#include "EvStatusInfoWidget.h"

#include "Button.h"
#include "Image.h"
#include "TextBlock.h"
#include "VerticalBox.h"

#include "PokeCore.h"
#include "Net/HttpActor.h"

void UEvStatusInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ConfirmButton)
	{
		ConfirmButton->OnClicked.AddUniqueDynamic(this, &UEvStatusInfoWidget::OnConfirmButtonClicked);
	}
}

void UEvStatusInfoWidget::OnOpen()
{
	Super::OnOpen();

	APokeCharacter* SelectedCharacter = GetSelectedCharacter();
	if (!ensure(SelectedCharacter))
	{
		return;
	}

	if (!EvStatusBox)
	{
		return;
	}

	EvStatusBox->ClearChildren();
	EvStatWidgets.Empty();
	UpAmount.Init(0, (int32)EStatus::Count);

	if (!EvStatWidgetClass.Get())
	{
		return;
	}

	RemainStatPoint = SelectedCharacter->GetRemainEvStatPoint();

	if (RemainStatPointText)
	{
		FString RemainPointString = FString(TEXT("남은 포인트: "));
		RemainPointString += FString::FromInt(RemainStatPoint);

		RemainStatPointText->SetText(FText::FromString(RemainPointString));
	}

	for (int32 StatIndex = 0; StatIndex < (int32)EStatus::Count; ++StatIndex)
	{
		UEvStatWidget* EvStatWidget = CreateWidget<UEvStatWidget>(GetWorld(), EvStatWidgetClass.Get());
		if (!EvStatWidget)
		{
			return;
		}

		EvStatWidget->InitSelectedCharacter(*SelectedCharacter);
		EvStatWidget->InitEvStatusInfoWidget(*this);
		EvStatWidget->InitEvStat((EStatus)StatIndex);

		EvStatusBox->AddChildToVerticalBox(EvStatWidget);
		EvStatWidgets.AddUnique(EvStatWidget);
	}
}

int32 UEvStatusInfoWidget::GetUpAmount(EStatus InStatus) const
{
	if (!UpAmount.IsValidIndex((int32)InStatus))
	{
		return 0;
	}

	return UpAmount[(int32)InStatus];
}

void UEvStatusInfoWidget::SetUpAmount(EStatus InStatus, int32 InChangeAmount)
{
	if (!UpAmount.IsValidIndex((int32)InStatus))
	{
		return;
	}

	UpAmount[(int32)InStatus] += InChangeAmount;

	RemainStatPoint -= InChangeAmount;

	if (RemainStatPointText)
	{
		FString RemainPointString = FString(TEXT("남은 포인트: "));
		RemainPointString += FString::FromInt(RemainStatPoint);

		RemainStatPointText->SetText(FText::FromString(RemainPointString));
	}
}

void UEvStatusInfoWidget::OnConfirmButtonClicked()
{
	AHttpActor* HttpActor = PokeCore::GetHttpActor(GetWorld());
	if (!HttpActor)
	{
		return;
	}

	APokeCharacter* SelectedCharacter = GetSelectedCharacter();
	if (!ensure(SelectedCharacter))
	{
		return;
	}

	SelectedCharacter->AddEvStat(UpAmount);

	TArray<int32> UpdateCharacters;
	UpdateCharacters.Add(SelectedCharacter->GetCharacterID());

	FHttpRequestParams Params;
	Params.RequestType = EHttpRequestType::UpdateCharacters;
	Params.MemberID = PokeCore::DeviceId;
	Params.UpdateCharacterIds = UpdateCharacters;

	HttpActor->Request(Params);

	OnOpen();
}

void UEvStatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (AddButton)
	{
		AddButton->OnClicked.AddUniqueDynamic(this, &UEvStatWidget::OnAddButtonClicked);
	}

	if (MinusButton)
	{
		MinusButton->OnClicked.AddUniqueDynamic(this, &UEvStatWidget::OnMinusButtonClicked);
	}
}

void UEvStatWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!SelectedCharacter)
	{
		return;
	}

	if (!InfoWidget)
	{
		return;
	}

	if (EvStatText)
	{
		int32 CurrentEvStat = SelectedCharacter->GetEvStat(EvStat);

		EvStatText->SetText(FText::FromString(FString::FromInt(CurrentEvStat)));
	}

	if (AddedStatText)
	{
		FString AddedStatString = FString(TEXT("(+"));
		AddedStatString += FString::FromInt(InfoWidget->GetUpAmount(EvStat));
		AddedStatString += FString(TEXT(")"));

		AddedStatText->SetText(FText::FromString(AddedStatString));
	}

}

void UEvStatWidget::InitSelectedCharacter(APokeCharacter& InCharacter)
{
	SelectedCharacter = &InCharacter;

	if (!SelectedCharacter)
	{
		ensure(0);
	}
}

void UEvStatWidget::InitEvStatusInfoWidget(UEvStatusInfoWidget& InInfoWidget)
{
	InfoWidget = &InInfoWidget;

	if (!InfoWidget)
	{
		ensure(0);
	}
}

void UEvStatWidget::InitEvStat(EStatus InEvStat)
{
	EvStat = InEvStat;

	if (EvStatNameText)
	{
		FString EvStatString = PokeCore::GetStatusString(EvStat);

		EvStatNameText->SetText(FText::FromString(EvStatString));
	}
}

void UEvStatWidget::OnAddButtonClicked()
{
	//OnAddStatButtonClicked.Broadcast((int32)EvStat);
	if (!InfoWidget)
	{
		return;
	}

	if (!SelectedCharacter)
	{
		return;
	}

	if (InfoWidget->GetCurrentRemainStatPoint() <= 0)
	{
		return;
	}

	InfoWidget->SetUpAmount(EvStat, 1);
}

void UEvStatWidget::OnMinusButtonClicked()
{
	if (!InfoWidget)
	{
		return;
	}

	if (!SelectedCharacter)
	{
		return;
	}

	if (InfoWidget->GetUpAmount(EvStat) <= 0)
	{
		return;
	}

	InfoWidget->SetUpAmount(EvStat, -1);
}
