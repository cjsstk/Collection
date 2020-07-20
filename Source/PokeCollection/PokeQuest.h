// Copyright Me. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CMSType.h"
#include "UObject/NoExportTypes.h"
#include "PokeQuest.generated.h"


struct FInitQuestParams
{
	int32 QuestKey = -1;
	
	EQuestObjectionType ObjectionType = EQuestObjectionType::ClearBattleCount;

	int32 DestNum = 0;

	int32 ParentQuestKey = -1;

	FText QuestDesc;
};

/**
 * 퀘스트 오브젝트
 */
UCLASS()
class POKECOLLECTION_API UPokeQuest : public UObject
{
	GENERATED_BODY()

public:
	void InitQuest(FInitQuestParams& InitParams);

	/** If bIsAdd is true, InCount Added to CurrNum else InCount override CurrNum */
	void Update(bool bIsAdd, int32 InCount);

	bool CanComplete() const { return CurrNum >= DestNum; }
	int32 GetQuestKey() const { return QuestKey; }
	EQuestObjectionType GetObjectionType() const { return ObjectionType; }
	const FText& GetQuestDesc() const { return QuestDesc; }
	int32 GetCurrentCount() const { return CurrNum; }
	int32 GetDestCount() const { return DestNum; }

private:
	int32 QuestKey = -1;

	EQuestObjectionType ObjectionType = EQuestObjectionType::ClearBattleCount;

	int32 DestNum = 0;
	int32 CurrNum = 0;

	int32 ParentQuestKey = -1;

	FText QuestDesc;
};
