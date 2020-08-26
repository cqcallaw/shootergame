// Copyright Epic Games, Inc.All Rights Reserved.
#include "ShooterTestControllerBasicDedicatedServerTest.h"
#include "ShooterGameInstance.h"

void UShooterTestControllerBasicDedicatedServerTest::OnTick(float TimeDelta)
{
	if (GetTimeInCurrentState() > 300)
	{
		UE_LOG(LogGauntlet, Error, TEXT("Failing boot test after 300 secs!"));
		EndTest(-1);
	}
}

void UShooterTestControllerBasicDedicatedServerTest::OnPostMapChange(UWorld* World)
{
	if (IsInGame())
	{
		EndTest(0);
	}
}
