// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Warrior : ModuleRules
{
	public Warrior(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"GameplayTags",
			"GameplayTasks", 
			"GameplayAbilities"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		PublicIncludePaths.AddRange(new string[]
		{
			"Warrior/Public/Characters",
			"Warrior/Public/Controllers",
			"Warrior/Public/Gamemodes",
			"Warrior/Public/DataAssets",
			"Warrior/Public/Components",
			"Warrior/Public/AnimInstances",
			"Warrior/Public/AbilitySystem",
			"Warrior/Public/Items"
		});
		
		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
