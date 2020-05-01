// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PokeCollection : ModuleRules
{
	public PokeCollection(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", 
			"HeadMountedDisplay", "Paper2D", "UMG", "Slate", "SlateCore",
			"Http",
			"Json",
			"JsonUtilities" });
	}
}
