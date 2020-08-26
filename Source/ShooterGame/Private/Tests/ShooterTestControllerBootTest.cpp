// Copyright Epic Games, Inc.All Rights Reserved.
#include "Tests/ShooterTestControllerBootTest.h"
#include "ShooterGameInstance.h"

bool UShooterTestControllerBootTest::IsBootProcessComplete() const
{
	static double StartTime = FPlatformTime::Seconds();
	const double TimeSinceStart = FPlatformTime::Seconds() - StartTime;

	if (TimeSinceStart >= TestDelay)
	{
		if (const UWorld* World = GetWorld())
		{
			if (const UShooterGameInstance* GameInstance = Cast<UShooterGameInstance>(GetWorld()->GetGameInstance()))
			{
				if (GameInstance->GetCurrentState() == ShooterGameInstanceState::WelcomeScreen ||
					GameInstance->GetCurrentState() == ShooterGameInstanceState::MainMenu)
				{
					return true;
				}
			}
		}
	}

	return false;
}