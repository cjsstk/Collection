// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameDialogWidget.h"

#include "Button.h"
#include "TextBlock.h"

#include "CMS.h"

void UInGameDialogWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (BackgroundButton)
	{
		BackgroundButton->OnClicked.AddUniqueDynamic(this, &UInGameDialogWidget::OnBackgroundButtonClicked);
	}
}

void UInGameDialogWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!DialogTalk.IsValidIndex(CurrentDialogIndex))
	{
		return;
	}

	DialogTextDelayAgeSeconds += InDeltaTime;
	if (DialogTextDelayAgeSeconds >= DialogTextDelaySeconds)
	{
		DialogTextDelayAgeSeconds = 0.0f;
		CurrentTextChopNum += 1;
	}

	FDialogTalk CurrentDialogTalk = DialogTalk[CurrentDialogIndex];
	
	FString Dialog = CurrentDialogTalk.TalkContext;
	
	CurrentTextLen = Dialog.Len();
	int32 PrintTextChopNum = (CurrentTextLen - CurrentTextChopNum);
	PrintTextChopNum = FMath::Clamp(PrintTextChopNum, 0, CurrentTextLen);

	FString PrintDialog = Dialog.LeftChop(PrintTextChopNum);

	if (DialogText)
	{
		DialogText->SetText(FText::FromString(PrintDialog));
	}

	if (TalkPersonName)
	{
		TalkPersonName->SetText(CurrentDialogTalk.TalkPersonName);
	}
}

void UInGameDialogWidget::InitDialog(int32 InDialogKey)
{
	const FDialogInfoTable* DialogTable = CMS::GetDialogDataTable(InDialogKey);
	if (ensure(DialogTable))
	{
		DialogTalk = DialogTable->DialogTalk;
	}
}

void UInGameDialogWidget::OnBackgroundButtonClicked()
{
	if ((CurrentTextLen - CurrentTextChopNum) <= 0)
	{
		CurrentDialogIndex++;
		DialogTextDelayAgeSeconds = 0.0f;
		CurrentTextChopNum = 0;

		if (!DialogTalk.IsValidIndex(CurrentDialogIndex))
		{
			RemoveFromViewport();
		}
	}
	else
	{
		CurrentTextChopNum = CurrentTextLen;
	}
}
