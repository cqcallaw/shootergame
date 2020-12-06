// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "ShooterTypes.h"
#include "OnlineLeaderboardInterface.h"

// these are normally exported from platform-specific tools
#define LEADERBOARD_STAT_SCORE			"ShooterAllTimeMatchResultsScore"
#define LEADERBOARD_STAT_KILLS			"ShooterAllTimeMatchResultsFrags"
#define LEADERBOARD_STAT_DEATHS			"ShooterAllTimeMatchResultsDeaths"
#define LEADERBOARD_STAT_MATCHESPLAYED	"ShooterAllTimeMatchResultsMatchesPlayed"

/**
 *	'AllTime' leaderboard read object
 */
class FShooterAllTimeMatchResultsRead : public FOnlineLeaderboardRead
{
public:

	FShooterAllTimeMatchResultsRead()
	{
		// Default properties
		LeaderboardName = FName(TEXT("ShooterAllTimeMatchResults"));
		SortedColumn = LEADERBOARD_STAT_SCORE;

		// Define default columns
		new (ColumnMetadata) FColumnMetaData(LEADERBOARD_STAT_SCORE, EOnlineKeyValuePairDataType::Int32);
	}
};

/**
 *	'AllTime' leaderboard write object
 */
class FShooterAllTimeMatchResultsWrite : public FOnlineLeaderboardWrite
{
public:

	FShooterAllTimeMatchResultsWrite()
	{
		// Default properties
		new (LeaderboardNames) FName(TEXT("ShooterAllTimeMatchResults"));
		RatedStat = LEADERBOARD_STAT_SCORE;
		DisplayFormat = ELeaderboardFormat::Number;
		SortMethod = ELeaderboardSort::Descending;
		UpdateMethod = ELeaderboardUpdateMethod::KeepBest;
	}
};

