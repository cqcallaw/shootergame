// Copyright Epic Games, Inc.All Rights Reserved.
#include "ShooterTestControllerDedicatedServerTest.h"
#include "ShooterGameSession.h"

void UShooterTestControllerDedicatedServerTest::OnTick(float TimeDelta)
{
	Super::OnTick(TimeDelta);

	if (bIsLoggedIn && !bIsSearchingForGame && !bFoundGame)
	{
		StartSearchingForGame();
	}

	if (bIsSearchingForGame && !bFoundGame)
	{
		UpdateSearchStatus();
	}
}