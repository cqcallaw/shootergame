// Copyright Epic Games, Inc.All Rights Reserved.
#pragma once

#include "ShooterTestControllerBase.h"
#include "ShooterTestControllerListenServerHost.generated.h"

UCLASS()
class UShooterTestControllerListenServerHost : public UShooterTestControllerBase
{
	GENERATED_BODY()

public:
	virtual void OnPostMapChange(UWorld* World) override {}

protected:
	virtual void OnUserCanPlayOnline(const FUniqueNetId& UserId, EUserPrivileges::Type Privilege, uint32 PrivilegeResults) override;
};