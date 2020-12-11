// Copyright Epic Games, Inc.All Rights Reserved.
#include "Tests/ShooterTestControllerListenServerQuickMatchClient.h"
#include "Online/ShooterGameSession.h"

void UShooterTestControllerListenServerQuickMatchClient::OnTick(float TimeDelta)
{
	Super::OnTick(TimeDelta);

	if (bIsLoggedIn && !bInQuickMatchSearch && !bFoundQuickMatchGame)
	{
		StartQuickMatch();
	}
}