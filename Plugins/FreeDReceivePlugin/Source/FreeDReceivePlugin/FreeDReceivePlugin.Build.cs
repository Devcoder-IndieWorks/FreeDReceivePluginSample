// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FreeDReceivePlugin : ModuleRules
{
	public FreeDReceivePlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		bEnforceIWYU = false;
		
		PrivateIncludePaths.AddRange(
		    new string[] {
				"FreeDReceivePlugin/Private"
			});

		PrivateDependencyModuleNames.AddRange(
		    new string[] {
				"Core", 
				"CoreUObject", 
				"Engine", 
				"InputCore"
			});
	}
}