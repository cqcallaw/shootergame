// Copyright 1998-2019 Epic Games, Inc.All Rights Reserved.
#include "Tests/ShooterTestControllerDedicatedServerTest.h"
#include "Online/ShooterGameSession.h"

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