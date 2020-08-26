// Copyright Epic Games, Inc.All Rights Reserved.
#pragma once

#include "ShooterTestControllerBase.h"
#include "SharedPointer.h"
#include "ShooterTestControllerDedicatedServerTest.generated.h"

UCLASS()
class UShooterTestControllerDedicatedServerTest : public UShooterTestControllerBase
{
	GENERATED_BODY()

protected:
	virtual void OnTick(float TimeDelta) override;
};