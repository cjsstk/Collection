// Copyright Me. All Rights Reserved.


#include "PokeQuest.h"

void UPokeQuest::InitQuest(FInitQuestParams& InitParams)
{
	QuestKey = InitParams.QuestKey;
	ObjectionType = InitParams.ObjectionType;
	DestNum = InitParams.DestNum;
	ParentQuestKey = InitParams.ParentQuestKey;
	QuestDesc = InitParams.QuestDesc;
}

void UPokeQuest::Update(bool bIsAdd, int32 InCount)
{
	if (bIsAdd)
	{
		CurrNum = FMath::Min(DestNum, CurrNum + InCount);
	}
	else
	{
		CurrNum = InCount;
	}
}

void UPokeQuest::Complete()
{
	bIsCompleted = true;
}
