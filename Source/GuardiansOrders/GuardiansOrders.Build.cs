// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GuardiansOrders : ModuleRules
{
	public GuardiansOrders(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "OnlineSubsystemSteam", "OnlineSubsystem", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput", "CommonInput", "CommonUI", "Slate", "SlateCore", "MultiplayerSessions" });

        PrivateIncludePaths.Add("GuardiansOrders");
        PublicIncludePaths.Add("GuardiansOrders");
    }
}
