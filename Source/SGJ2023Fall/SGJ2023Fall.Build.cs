// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SGJ2023Fall : ModuleRules
{
	public SGJ2023Fall(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
