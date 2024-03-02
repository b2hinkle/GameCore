// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class GameCore : ModuleRules
{
    public GameCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PrivatePCHHeaderFile = "Private/GameCorePrivatePCH.h";

        PublicDependencyModuleNames.AddRange(new string[] { "Core" });
        PrivateDependencyModuleNames.AddRange(new string[] { "CoreUObject", "Engine" });

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "NetCore", // for push model
                "Projects"
            }
        );
    }
}
