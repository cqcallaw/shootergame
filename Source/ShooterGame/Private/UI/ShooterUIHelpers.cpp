// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterGame.h"
#include "ShooterUIHelpers.h"
#include "OnlineSubsystemUtils.h"

FText ShooterUIHelpers::GetProfileOpenText() const
{
	// @todo: replace button with icon
	// @todo: replace 'GamerCard' with distribution specific terminology (Steam, Origin, UPlay, etc)
#if SHOOTER_XBOX_STRINGS
	return NSLOCTEXT("Network", "XB1OpenProfile", "Press A for GamerCard");
#elif PLATFORM_PS4
	return NSLOCTEXT("Network", "PS4OpenProfile", "Press cross button for GamerCard");
#else
	return NSLOCTEXT("Network", "PCOpenProfile", "Press Enter for GamerCard");
#endif
}

bool ShooterUIHelpers::ProfileOpenedUI(UWorld* World, const FUniqueNetId& Requestor, const FUniqueNetId& Requestee, const FOnProfileUIClosedDelegate* Delegate) const
{
	const IOnlineSubsystem* OnlineSub = Online::GetSubsystem(World);
	if (OnlineSub)
	{
		// Show the profile UI.
		const IOnlineExternalUIPtr ExternalUI = OnlineSub->GetExternalUIInterface();
		if (ExternalUI.IsValid())
		{
			// Create a dummy delegate, if one wasn't specified
			struct Local
			{
				static void DummyOnProfileOpenedUIClosedDelegate()
				{
					// do nothing
				}
			};
			return ExternalUI->ShowProfileUI(Requestor, Requestee, Delegate ? *Delegate : FOnProfileUIClosedDelegate::CreateStatic(&Local::DummyOnProfileOpenedUIClosedDelegate) );
		}
	}
	return false;
}

FText ShooterUIHelpers::GetProfileSwapText() const
{
	// @todo: replace button with icon
#if SHOOTER_XBOX_STRINGS
	return NSLOCTEXT("Network", "XB1SwapProfile", "Y Switch User");
/*#elif PLATFORM_PS4
	return NSLOCTEXT("Network", "PS4SwapProfile", "Triangle button Switch User");
*/
#else
	return NSLOCTEXT("Network", "PCSwapProfile", "Space - Switch User");
#endif
}

bool ShooterUIHelpers::ProfileSwapUI(UWorld* World, const int ControllerIndex, bool bShowOnlineOnly, const FOnLoginUIClosedDelegate* Delegate) const
{
	const IOnlineSubsystem* OnlineSub = Online::GetSubsystem(World);
	if (OnlineSub)
	{
		// Show the profile UI.
		const IOnlineExternalUIPtr ExternalUI = OnlineSub->GetExternalUIInterface();
		if (ExternalUI.IsValid())
		{
			// Create a dummy delegate, if one wasn't specified
			struct Local
			{
				static void DummyOnProfileSwapUIClosedDelegate(TSharedPtr<const FUniqueNetId> UniqueId, const int InControllerIndex, const FOnlineError& Error)
				{
					// do nothing
				}
			};
			return ExternalUI->ShowLoginUI(ControllerIndex, bShowOnlineOnly, false, Delegate ? *Delegate : FOnLoginUIClosedDelegate::CreateStatic(&Local::DummyOnProfileSwapUIClosedDelegate) );
		}
	}
	return false;
}