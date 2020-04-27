// Fill out your copyright notice in the Description page of Project Settings.


#include "PokeSkill.h"

#include "Engine/World.h"

#include "BattleCharacterActor.h"

void UPokeSkill::UseSkill(const FPokeUseSkillParams& Params)
{
	OnUseSkill(Params);
}

void UPokeSkill::OnUseSkill(const FPokeUseSkillParams& Params)
{
	if (!ensure(Params.SourceCharacter))
	{
		return;
	}

	if (Params.SpawnActor.Get())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = Cast<APawn>(Params.SourceCharacter);
		GetWorld()->SpawnActor<AActor>(Params.SpawnActor.Get(), Params.SourceCharacter->GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
	}
}

void UPokeSkill_Ember::OnUseSkill(const FPokeUseSkillParams& Params)
{
	Super::OnUseSkill(Params);


}
