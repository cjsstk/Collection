// Copyright Me. All Rights Reserved.


#include "ChangeEquipmentInfoPopUp.h"

#include "Button.h"
#include "Image.h"
#include "TextBlock.h"
#include "VerticalBox.h"

#include "PokeCharacter.h"
#include "PokeCollectionCharacter.h"
#include "PokeCollectionHUD.h"
#include "PokeEquipment.h"
#include "Widgets/InGameChangeEquipmentWidget.h"
#include "Widgets/InGameCharacterInfoWidget.h"

void UChangeEquipmentInfoPopUp::NativeConstruct()
{
	Super::NativeConstruct();

	if (CancelButton)
	{
		CancelButton->OnClicked.AddUniqueDynamic(this, &UChangeEquipmentInfoPopUp::OnCancelButtonClicked);
	}

	if (ChangeButton)
	{
		ChangeButton->OnClicked.AddUniqueDynamic(this, &UChangeEquipmentInfoPopUp::OnChangeButtonClicked);
	}
}

void UChangeEquipmentInfoPopUp::InitInfo(int32 InNextEquipmentID)
{
	APokeCollectionCharacter* Player = Cast<APokeCollectionCharacter>(GetOwningPlayerPawn());
	if (!ensure(Player) || !GetOwningPlayer())
	{
		return;
	}

	APokeCollectionHUD* PokeHud = Cast<APokeCollectionHUD>(GetOwningPlayer()->GetHUD());
	if (!PokeHud)
	{
		return;
	}

	UInGameCharacterInfoWidget* InfoWidget = PokeHud->GetInGameCharacterInfoWidget();
	if (!InfoWidget)
	{
		return;
	}

	APokeCharacter* SelectedCharacter = Player->GetCharacterByID(InfoWidget->GetSelectedCharacterID());
	if (!ensure(SelectedCharacter))
	{
		return;
	}

	UPokeEquipment* CurrentEquipment = SelectedCharacter->GetCurrentEquipment();

	CurrentEquipmentID = CurrentEquipment ? CurrentEquipment->GetEquipmentID() : -1;
	NextEquipmentID = InNextEquipmentID;

	UPokeEquipment* NextEquipment = Player->GetEquipmentByID(NextEquipmentID);

	InitCurrentEquipmentInfo(CurrentEquipment);
	InitNextEquipmentInfo(NextEquipment);
}

void UChangeEquipmentInfoPopUp::OnCancelButtonClicked()
{
	RemoveFromViewport();
}

void UChangeEquipmentInfoPopUp::OnChangeButtonClicked()
{
	AHttpActor* HttpActor = PokeCore::GetHttpActor(GetWorld());
	if (!HttpActor)
	{
		return;
	}

	APokeCollectionCharacter* Player = Cast<APokeCollectionCharacter>(GetOwningPlayerPawn());
	if (!ensure(Player) || !GetOwningPlayer())
	{
		return;
	}

	APokeCollectionHUD* PokeHud = Cast<APokeCollectionHUD>(GetOwningPlayer()->GetHUD());
	if (!PokeHud)
	{
		return;
	}

	UInGameCharacterInfoWidget* InfoWidget = PokeHud->GetInGameCharacterInfoWidget();
	if (!InfoWidget)
	{
		return;
	}

	Player->PutOnEquipment(InfoWidget->GetSelectedCharacterID(), NextEquipmentID);
	
	TArray<int32> UpdatedEquipIds;

	if (Player->GetEquipmentByID(CurrentEquipmentID))
	{
		UpdatedEquipIds.Add(CurrentEquipmentID);
	}

	if (Player->GetEquipmentByID(NextEquipmentID))
	{
		UpdatedEquipIds.Add(NextEquipmentID);
	}

	FHttpRequestParams Params;
	Params.RequestType = EHttpRequestType::UpdateEquipments;
	Params.MemberID = PokeCore::DeviceId;
	Params.UpdateEquipmentIds = UpdatedEquipIds;

	HttpActor->Request(Params);

	PokeHud->OnBackButtonClicked(PokeHud->GetInGameChangeEquipmentWidget());

	RemoveFromViewport();
}

void UChangeEquipmentInfoPopUp::InitCurrentEquipmentInfo(class UPokeEquipment* InCurrentEquipment)
{
	if (!CurrentEquipmentInfo)
	{
		return;
	}

	CurrentEquipmentInfo->ClearChildren();

	if (InCurrentEquipment)
	{
		CurrentEquipmentImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		CurrentEquipmentImage->SetBrushFromTexture(InCurrentEquipment->GetEquipmentProfileImage());
	}
	else
	{
		CurrentEquipmentImage->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	if (!ChangeEquipmentStatusSlotClass.Get())
	{
		return;
	}

	const TArray<int32>& EquipmentStatus = InCurrentEquipment->GetFinalEquipmentStatus2();

	for (int32 StatusIndex = 0; StatusIndex < EquipmentStatus.Num(); ++StatusIndex)
	{
		EEquipmentStatus StatusEnum = (EEquipmentStatus)StatusIndex;
		FString StatusName;
		int32 StatusValue = EquipmentStatus[StatusIndex];

		if (StatusValue <= 0)
		{
			continue;
		}

		UChangeEquipmentStatusSlot* StatusSlot = CreateWidget<UChangeEquipmentStatusSlot>(GetWorld(), ChangeEquipmentStatusSlotClass.Get());
		if (!ensure(StatusSlot))
		{
			continue;
		}

		switch (StatusEnum)
		{
		case EEquipmentStatus::Attack:
			StatusSlot->SetStatusText(FString(TEXT("공격")), StatusValue);
			break;
		case EEquipmentStatus::Defense:
			StatusSlot->SetStatusText(FString(TEXT("방어")), StatusValue);
			break;
		case EEquipmentStatus::SpecialAttack:
			StatusSlot->SetStatusText(FString(TEXT("특공")), StatusValue);
			break;
		case EEquipmentStatus::SpecialDefense:
			StatusSlot->SetStatusText(FString(TEXT("특방")), StatusValue);
			break;
		case EEquipmentStatus::Speed:
			StatusSlot->SetStatusText(FString(TEXT("스피드")), StatusValue);
			break;
		case EEquipmentStatus::AttackRange:
			StatusSlot->SetStatusText(FString(TEXT("사거리")), StatusValue);
			break;
		default:
			ensure(0);
			return;
		}

		CurrentEquipmentInfo->AddChildToVerticalBox(StatusSlot);
	}

}

void UChangeEquipmentInfoPopUp::InitNextEquipmentInfo(class UPokeEquipment* InNextEquipment)
{
	if (!NextEquipmentInfo)
	{
		return;
	}

	NextEquipmentInfo->ClearChildren();

	if (InNextEquipment)
	{
		NextEquipmentImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		NextEquipmentImage->SetBrushFromTexture(InNextEquipment->GetEquipmentProfileImage());
	}
	else
	{
		NextEquipmentImage->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	if (!ChangeEquipmentStatusSlotClass.Get())
	{
		return;
	}

	const TArray<int32>& EquipmentStatus = InNextEquipment->GetFinalEquipmentStatus2();

	for (int32 StatusIndex = 0; StatusIndex < EquipmentStatus.Num(); ++StatusIndex)
	{
		EEquipmentStatus StatusEnum = (EEquipmentStatus)StatusIndex;
		FString StatusName;
		int32 StatusValue = EquipmentStatus[StatusIndex];

		if (StatusValue <= 0)
		{
			continue;
		}

		switch (StatusEnum)
		{
		case EEquipmentStatus::Attack:
			StatusName = FString(TEXT("공격"));
			break;
		case EEquipmentStatus::Defense:
			StatusName = FString(TEXT("방어"));
			break;
		case EEquipmentStatus::SpecialAttack:
			StatusName = FString(TEXT("특공"));
			break;
		case EEquipmentStatus::SpecialDefense:
			StatusName = FString(TEXT("특방"));
			break;
		case EEquipmentStatus::Speed:
			StatusName = FString(TEXT("스피드"));
			break;
		case EEquipmentStatus::AttackRange:
			StatusName = FString(TEXT("사거리"));
			break;
		default:
			ensure(0);
			return;
		}

		UChangeEquipmentStatusSlot* StatusSlot = CreateWidget<UChangeEquipmentStatusSlot>(GetWorld(), ChangeEquipmentStatusSlotClass.Get());
		if (ensure(StatusSlot))
		{
			StatusSlot->SetStatusText(StatusName, StatusValue);
			NextEquipmentInfo->AddChildToVerticalBox(StatusSlot);
		}
	}
}

void UChangeEquipmentStatusSlot::SetStatusText(FString InStatusType, int32 InChangeStatus)
{
	if (!ensure(ChangeStatusText))
	{
		return;
	}

	FString StatusText = InStatusType;
	StatusText += TEXT("  +");
	StatusText += FString::FromInt(InChangeStatus);

	ChangeStatusText->SetText(FText::FromString(StatusText));
}
