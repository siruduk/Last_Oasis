// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Last_Oasis : ModuleRules
{
	public Last_Oasis(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicIncludePaths.AddRange(new string[] { "Last_Oasis" });
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "CinematicCamera", "AIModule" });
		PrivateDependencyModuleNames.AddRange(new string[] {"GameFeatures", "GameplayAbilities", "GameplayTasks","GameplayTags"});

	}
}
