// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ACTGame : ModuleRules
{
	public ACTGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Slate",
            "SlateCore",
            "GameplayAbilities",
            "GameplayTags",
            "GameplayTasks" });
	}
}
