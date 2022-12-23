// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Ro3ot : ModuleRules
{
	public Ro3ot(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "NavigationSystem", "AIModule", "Niagara" });
	}
}
