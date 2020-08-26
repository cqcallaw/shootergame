// Copyright Epic Games, Inc.All Rights Reserved.
#pragma once

#include "ShooterTestControllerBase.h"
#include "ShooterTestControllerListenServerQuickMatchClient.generated.h"

UCLASS()
class UShooterTestControllerListenServerQuickMatchClient : public UShooterTestControllerBase
{
	GENERATED_BODY()

protected:
	virtual void OnTick(float TimeDelta) override;
};