// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GB_Tanks_UE5 : ModuleRules
{
	public GB_Tanks_UE5(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", });

		PrivateDependencyModuleNames.AddRange(new string[] {  });
		
		// Add dependency to our Inventory System Plugin.
		PublicDependencyModuleNames.AddRange(new string[] { "InventorySystem", });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
