// Copyright Epic Games, Inc.All Rights Reserved.
#include "ShooterTestControllerListenServerQuickMatchClient.h"
#include "ShooterGameSession.h"

void UShooterTestControllerListenServerQuickMatchClient::OnTick(float TimeDelta)
{
	Super::OnTick(TimeDelta);

	if (bIsLoggedIn && !bInQuickMatchSearch && !bFoundQuickMatchGame)
	{
		StartQuickMatch();
	}
}