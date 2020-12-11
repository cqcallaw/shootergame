// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ShooterGameEditorTarget : TargetRules
{
	public ShooterGameEditorTarget(TargetInfo Target) : base(Target)
	{
		DefaultBuildSettings = BuildSettingsVersion.V2;
		bLegacyPublicIncludePaths = true;
		ShadowVariableWarningLevel = WarningLevel.Error;
		Type = TargetType.Editor;

		ExtraModuleNames.Add("ShooterGame");
	}
}
