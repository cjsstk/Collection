// Copyright Me. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/InGameCharacterInfoWidget.h"
#include "CharacterSkillInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class POKECOLLECTION_API UCharacterSkillInfoWidget : public UCharacterInfoContentWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void OnOpen() override;
};
