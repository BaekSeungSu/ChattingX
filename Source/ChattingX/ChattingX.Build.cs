// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ChattingX : ModuleRules
{
	public ChattingX(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
			{ "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput",
				
				// UI
				"UMG", "Slate", "SlateCore" 
			});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		PublicIncludePaths.AddRange(new string[] { "ChattingX" });
	}
}
