// Copyright 1998-2019 Epic Games, Inc.All Rights Reserved.
#include "Tests/ShooterTestControllerListenServerHost.h"

void UShooterTestControllerListenServerHost::OnUserCanPlayOnline(const FUniqueNetId& UserId, EUserPrivileges::Type Privilege, uint32 PrivilegeResults)
{
	Super::OnUserCanPlayOnline(UserId, Privilege, PrivilegeResults);

	if (PrivilegeResults == (uint32)IOnlineIdentity::EPrivilegeResults::NoFailures)
	{
		HostGame();
	}
}