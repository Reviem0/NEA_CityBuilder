// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;
using System;

public class NEA2 : ModuleRules
{

	private string ThirdPartyPath
	{ 
		get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "../../ThirdParty/")); } 
	}
	public bool LoadCryptopp(ReadOnlyTargetRules Target){
		bool isLibrarySupported = false;
		if ((Target.Platform == UnrealTargetPlatform.Win64)){
			isLibrarySupported = true;
			string LibrariesPath = Path.Combine(ThirdPartyPath, "cryptopp", "Libraries");
			PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "cryptlib.lib"));
		}
		if (isLibrarySupported){
			PublicIncludePaths.AddRange(new string[] {Path.Combine(ThirdPartyPath, "cryptopp", "Includes")});

		}
		return isLibrarySupported;
	}
	public NEA2(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		bUseRTTI = true;
        bEnableUndefinedIdentifierWarnings = false;
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "DatabaseSupport", "SQLiteSupport", "SQLiteCore"});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
		LoadCryptopp(Target);
	}
}
