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

void UPokeQuest::Update(int32 InCount)
{
	CurrNum = FMath::Min(DestNum, CurrNum + InCount);
}
