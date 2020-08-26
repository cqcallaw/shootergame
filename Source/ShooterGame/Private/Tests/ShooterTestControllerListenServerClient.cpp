// Copyright Epic Games, Inc.All Rights Reserved.
#include "ShooterTestControllerListenServerClient.h"
#include "ShooterGameSession.h"

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