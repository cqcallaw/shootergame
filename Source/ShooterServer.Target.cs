// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

[SupportedPlatforms(UnrealPlatformClass.Server)]
public class ShooterServerTarget : TargetRules
{
	public ShooterServerTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Server;
		bUsesSteam = true;

		ExtraModuleNames.Add("ShooterGame");
	}
}
