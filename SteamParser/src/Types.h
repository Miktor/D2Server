#ifndef Types_h__
#define Types_h__

#include "stdafx.h"
#include <cpprest/basic_types.h>

namespace SteamParser
{
	struct MatchHistoryBySeqNumberResult
	{
		struct AbilityUpgrade
		{
			uint32_t ability = 0;
			uint32_t time = 0;
			uint8_t  level = 0;
		};

		struct AdditionalUnit
		{
			utility::string_t	unitName;
			uint16_t			item0 = 0;
			uint16_t			item1 = 0;
			uint16_t			item2 = 0;
			uint16_t			item3 = 0;
			uint16_t			item4 = 0;
			uint16_t			item5 = 0;
		};

		struct Player
		{
			uint32_t						accountId = 0;
			uint8_t							playerSlot = 0;
			uint8_t							heroId = 0;
			uint16_t						item0 = 0;
			uint16_t						item1 = 0;
			uint16_t						item2 = 0;
			uint16_t						item3 = 0;
			uint16_t						item4 = 0;
			uint16_t						item5 = 0;
			uint8_t							kills = 0;
			uint8_t							deaths = 0;
			uint8_t							assists = 0;
			uint8_t							leaverStatus = 0;
			uint32_t						gold = 0;
			uint16_t						lastHits = 0;
			uint8_t							denies = 0;
			uint16_t						gpm = 0;
			uint16_t						xpm = 0;
			uint32_t						goldSpent = 0;
			uint32_t						heroDamage = 0;
			uint32_t						towerDamage = 0;
			uint32_t						heroHealing = 0;
			uint8_t							level = 0;
			std::vector<AbilityUpgrade>	abilityUpgrades;
			std::vector<AdditionalUnit>	additionalUnits;
		};

		struct PicksBans
		{
			bool	isPick = false;
			uint8_t	heroId = 0;
			uint8_t team = 0;
			uint8_t order = 0;
		};

		struct Match
		{
			std::vector<Player>		players;
			uint64_t				season = 0;
			bool					radiantWin = 0;
			uint32_t				durration = 0;
			uint32_t				startTime = 0;
			uint64_t				matchId = 0;
			uint64_t				matchSeq = 0;
			uint32_t				cluster = 0;
			uint32_t				firstbloodTime = 0;
			uint32_t				lobbyType = 0;
			uint32_t				humanPlayers = 0;
			uint32_t				leagueId = 0;
			uint32_t				positiveVotes = 0;
			uint32_t				negativeVotes = 0;
			uint32_t				gameMode = 0;
			uint32_t				towerStatusRadiant = 0;
			uint32_t				towerStatusDire = 0;
			uint32_t				barracksStatusRadiant = 0;
			uint32_t				barracksStatusDire = 0;
			uint32_t				radiantCaptain = 0;
			uint32_t				direCaptain = 0;
			uint32_t				radiantTeamid = 0;
			uint32_t				direTeamid = 0;
			uint32_t				radiantGuildId = 0;
			uint32_t				direGuildId = 0;
			uint32_t				radiantGuildLogo = 0;
			uint32_t				direGuildLogo = 0;
			uint32_t				radiantLogo = 0;
			uint32_t				direLogo = 0;
			uint32_t				radiantTeamComplete = 0;
			uint32_t				direTeamComplete = 0;
			utility::string_t		radiantGuildName;
			utility::string_t		direGuildName;
			utility::string_t		radiantName;
			utility::string_t		direName;
			std::vector<PicksBans>	picksBans;
		};

		uint32_t status = -1;
		std::vector<Match> matches;
	};
}
#endif // Types_h__
