// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "ParticleDefinitions.h"
#include "SoundDefinitions.h"
#include "Net/UnrealNetwork.h"
#include "ShooterGameMode.h"
#include "ShooterGameState.h"
#include "ShooterCharacter.h"
#include "ShooterCharacterMovement.h"
#include "ShooterPlayerController.h"
#include "ShooterGameClasses.h"


class UBehaviorTreeComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogShooter, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogShooterWeapon, Log, All);

/** when you modify this, please note that this information can be saved with instances
 * also DefaultEngine.ini [/Script/Engine.CollisionProfile] should match with this list **/
#define COLLISION_WEAPON		ECC_GameTraceChannel1
#define COLLISION_PROJECTILE	ECC_GameTraceChannel2
#define COLLISION_PICKUP		ECC_GameTraceChannel3

#define MAX_PLAYER_NAME_LENGTH 16


#ifndef SHOOTER_CONSOLE_UI
/** Set to 1 to pretend we're building for console even on a PC, for testing purposes */
#define SHOOTER_SIMULATE_CONSOLE_UI	0

#if PLATFORM_PS4 || PLATFORM_SWITCH || SHOOTER_SIMULATE_CONSOLE_UI
	#define SHOOTER_CONSOLE_UI 1
#else
	#define SHOOTER_CONSOLE_UI 0
#endif
#endif

#ifndef SHOOTER_XBOX_STRINGS
	#define SHOOTER_XBOX_STRINGS 0
#endif

#ifndef SHOOTER_SHOW_QUIT_MENU_ITEM
	#define SHOOTER_SHOW_QUIT_MENU_ITEM (!SHOOTER_CONSOLE_UI)
#endif

#ifndef SHOOTER_SUPPORTS_OFFLINE_SPLIT_SCREEEN
	#define SHOOTER_SUPPORTS_OFFLINE_SPLIT_SCREEEN 1
#endif

// whether the platform will signal a controller pairing change on a controller disconnect. if not, we need to treat the pairing change as a request to switch profiles when the destination profile is not specified
#ifndef SHOOTER_CONTROLLER_PAIRING_ON_DISCONNECT
	#define SHOOTER_CONTROLLER_PAIRING_ON_DISCONNECT 1
#endif

// whether the game should display an account picker when a new input device is connected, while the "please reconnect controller" message is on screen.
#ifndef SHOOTER_CONTROLLER_PAIRING_PROMPT_FOR_NEW_USER_WHEN_RECONNECTING
	#define SHOOTER_CONTROLLER_PAIRING_PROMPT_FOR_NEW_USER_WHEN_RECONNECTING 0
#endif
