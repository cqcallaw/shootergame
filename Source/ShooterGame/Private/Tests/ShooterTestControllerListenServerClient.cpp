// Copyright Epic Games, Inc.All Rights Reserved.
#include "Tests/ShooterTestControllerListenServerClient.h"
#include "Online/ShooterGameSession.h"

void UShooterTestControllerListenServerClient::OnTick(float TimeDelta)
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