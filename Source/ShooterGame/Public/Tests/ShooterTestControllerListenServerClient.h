// Copyright Epic Games, Inc.All Rights Reserved.
#pragma once

#include "ShooterTestControllerBase.h"
#include "ShooterTestControllerListenServerClient.generated.h"

UCLASS()
class UShooterTestControllerListenServerClient : public UShooterTestControllerBase
{
	GENERATED_BODY()

protected:
	virtual void OnTick(float TimeDelta) override;
};