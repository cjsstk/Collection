// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class ABattleManager;

namespace PokeCore
{
    class ABattleManager* GetBattleManager(const UWorld* WorldContext);

};