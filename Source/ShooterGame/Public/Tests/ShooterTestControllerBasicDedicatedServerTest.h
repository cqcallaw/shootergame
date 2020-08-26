// Copyright Epic Games, Inc.All Rights Reserved.
#pragma once

#include "Tests/ShooterTestControllerBase.h"
#include "ShooterTestControllerBasicDedicatedServerTest.generated.h"

UCLASS()
class UShooterTestControllerBasicDedicatedServerTest : public UShooterTestControllerBase
{
	GENERATED_BODY()

protected:
	virtual void OnTick(float TimeDelta) override;

public:
	virtual void OnPostMapChange(UWorld* World) override;
};