#include "stdafx.h"
#include "JSONParser.h"
#include "DotaAPI.h"

#define DEFINE_PARSER_EX(name) \
	static const auto name = std::map < utility::string_t, std::function<void(ResType&, const web::json::value&)> >

#define DEFINE_PARSER DEFINE_PARSER_EX(parsers)

#define MAKE_PARSER(name, lambda) \
	std::make_pair(U(name), [](ResType& res, const web::json::value& val) lambda)

namespace SteamParser{

	namespace JSON
	{
		template<class T>
		void SteamParser::JSON::JSONParser::ParseObject(T& object, const web::json::object &json, std::map<utility::string_t, std::function<void(T&, const web::json::value&)>> parsers)
		{
			for (auto it = json.cbegin(); it != json.cend(); ++it)
			{
				auto findIt = parsers.find(it->first);
				if (findIt != parsers.end())
				{
					findIt->second(object, it->second);
				}
				else
					throw std::runtime_error(std::string("Unknown json member: ") + utility::conversions::to_utf8string(it->first));
			}
		}


		MatchHistoryBySeqNumberResult JSONParser::ParseMatchHistoryBySeqNumber(web::json::value &json)
		{
			typedef MatchHistoryBySeqNumberResult ResType;

			DEFINE_PARSER
			{
				MAKE_PARSER("status",  { res.status = val.as_number().to_uint32(); }),
					MAKE_PARSER("matches",  { auto& matches = val.as_array();
					res.matches.reserve(matches.size());

					for (auto matchIt = matches.cbegin(); matchIt != matches.cend(); ++matchIt)
					{
						res.matches.push_back(GetMatch(matchIt->as_object()));
					}
				})
			};

			ResType deserialized;
			auto str = utility::string_t(U("result"));

			auto& obj = json.as_object();
			auto& result = obj[str].as_object();

			ParseObject(deserialized, result, parsers);

			return deserialized;

		}

		MatchHistoryBySeqNumberResult::Match JSONParser::GetMatch(const web::json::object &json)
		{
			typedef MatchHistoryBySeqNumberResult::Match ResType; 
			
			DEFINE_PARSER
			{
				MAKE_PARSER("players",
				{ 
					auto& players = val.as_array();
					res.players.reserve(players.size());
					for (auto playerIt = players.cbegin(); playerIt != players.cend(); ++playerIt)
					{
						res.players.push_back(GetPlayer(playerIt->as_object()));
					}
				}),
					MAKE_PARSER("season", { res.season = val.as_number().to_uint64(); }),
					MAKE_PARSER("radiant_win", { res.radiantWin = val.as_bool(); }),
					MAKE_PARSER("duration", { res.durration = val.as_number().to_uint32(); }),
					MAKE_PARSER("start_time", { res.startTime = val.as_number().to_uint32(); }),
					MAKE_PARSER("match_id", { res.matchId = val.as_number().to_uint64(); }),
					MAKE_PARSER("match_seq_num", { res.matchSeq = val.as_number().to_uint64(); }),
					MAKE_PARSER("cluster", { res.cluster = val.as_number().to_uint32(); }),
					MAKE_PARSER("first_blood_time", { res.firstbloodTime = val.as_number().to_uint32(); }),
					MAKE_PARSER("lobby_type", { res.lobbyType = val.as_number().to_uint32(); }),
					MAKE_PARSER("human_players", { res.humanPlayers = val.as_number().to_uint32(); }),
					MAKE_PARSER("leagueid", { res.leagueId = val.as_number().to_uint32(); }),
					MAKE_PARSER("positive_votes", { res.positiveVotes = val.as_number().to_uint32(); }),
					MAKE_PARSER("negative_votes", { res.negativeVotes = val.as_number().to_uint32(); }),
					MAKE_PARSER("game_mode", { res.gameMode = val.as_number().to_uint32(); }),
					MAKE_PARSER("tower_status_radiant", { res.towerStatusRadiant = val.as_number().to_uint32(); }),
					MAKE_PARSER("tower_status_dire", { res.towerStatusDire = val.as_number().to_uint32(); }),
					MAKE_PARSER("barracks_status_radiant", { res.barracksStatusRadiant = val.as_number().to_uint32(); }),
					MAKE_PARSER("barracks_status_dire", { res.barracksStatusDire = val.as_number().to_uint32(); }),
					MAKE_PARSER("radiant_captain", { res.radiantCaptain = val.as_number().to_uint32(); }),
					MAKE_PARSER("dire_captain", { res.direCaptain = val.as_number().to_uint32(); }),
					MAKE_PARSER("radiant_team_id", { res.radiantTeamid = val.as_number().to_uint32(); }),
					MAKE_PARSER("dire_team_id", { res.direTeamid = val.as_number().to_uint32(); }),
					MAKE_PARSER("radiant_guild_id", { res.radiantGuildId = val.as_number().to_uint32(); }),
					MAKE_PARSER("dire_guild_id", { res.direGuildId = val.as_number().to_uint32(); }),
					MAKE_PARSER("radiant_guild_logo", { res.radiantGuildLogo = val.as_number().to_uint32(); }),
					MAKE_PARSER("dire_guild_logo", { res.direGuildLogo = val.as_number().to_uint32(); }),
					MAKE_PARSER("radiant_guild_name", { res.radiantGuildName = val.as_string(); }),
					MAKE_PARSER("dire_guild_name", { res.direGuildName = val.as_string(); }),
					MAKE_PARSER("radiant_name", { res.radiantName = val.as_string(); }),
					MAKE_PARSER("dire_name", { res.direName = val.as_string(); }),
					MAKE_PARSER("radiant_logo", { res.radiantLogo = val.as_number().to_uint32(); }),
					MAKE_PARSER("dire_logo", { res.direLogo = val.as_number().to_uint32(); }),
					MAKE_PARSER("radiant_team_complete", { res.radiantTeamComplete = val.as_number().to_uint32(); }),
					MAKE_PARSER("dire_team_complete", { res.direTeamComplete = val.as_number().to_uint32(); }),
					MAKE_PARSER("picks_bans", 
				{ 
					auto& pbs = val.as_array();
					res.picksBans.reserve(pbs.size());

					for (auto bpsIt = pbs.cbegin(); bpsIt != pbs.cend(); ++bpsIt)
					{
						auto& pb = bpsIt->as_object();
						res.picksBans.push_back(GetPickban(pb));						
					}
				})
			};

			ResType match;
			ParseObject(match, json, parsers);
			return match;
		}

		MatchHistoryBySeqNumberResult::Player JSONParser::GetPlayer(const web::json::object &json)
		{
			typedef MatchHistoryBySeqNumberResult::Player ResType;

			DEFINE_PARSER
			{
					MAKE_PARSER("account_id", { res.accountId = val.as_number().to_uint32(); }),
					MAKE_PARSER("player_slot", { res.playerSlot = val.as_number().to_uint32(); }),
					MAKE_PARSER("hero_id", { res.heroId = val.as_number().to_uint32(); }),
					MAKE_PARSER("item_0", { res.item0 = val.as_number().to_uint32(); }),
					MAKE_PARSER("item_1", { res.item1 = val.as_number().to_uint32(); }),
					MAKE_PARSER("item_2", { res.item2 = val.as_number().to_uint32(); }),
					MAKE_PARSER("item_3", { res.item3 = val.as_number().to_uint32(); }),
					MAKE_PARSER("item_4", { res.item4 = val.as_number().to_uint32(); }),
					MAKE_PARSER("item_5", { res.item5 = val.as_number().to_uint32(); }),
					MAKE_PARSER("kills", { res.kills = val.as_number().to_uint32(); }),
					MAKE_PARSER("deaths", { res.deaths = val.as_number().to_uint32(); }),
					MAKE_PARSER("assists", { res.assists = val.as_number().to_uint32(); }),
					MAKE_PARSER("leaver_status", { res.leaverStatus = val.as_number().to_uint32(); }),
					MAKE_PARSER("gold", { res.gold = val.as_number().to_uint32(); }),
					MAKE_PARSER("last_hits", { res.lastHits = val.as_number().to_uint32(); }),
					MAKE_PARSER("denies", { res.denies = val.as_number().to_uint32(); }),
					MAKE_PARSER("gold_per_min", { res.gpm = val.as_number().to_uint32(); }),
					MAKE_PARSER("xp_per_min", { res.xpm = val.as_number().to_uint32(); }),
					MAKE_PARSER("gold_spent", { res.goldSpent = val.as_number().to_uint32(); }),
					MAKE_PARSER("hero_damage", { res.heroDamage = val.as_number().to_uint32(); }),
					MAKE_PARSER("tower_damage", { res.towerDamage = val.as_number().to_uint32(); }),
					MAKE_PARSER("hero_healing", { res.heroHealing = val.as_number().to_uint32(); }),
					MAKE_PARSER("level", { res.level = val.as_number().to_uint32(); }),
					MAKE_PARSER("ability_upgrades",
				{
					auto& abilities = val.as_array();
					res.abilityUpgrades.reserve(abilities.size());

					for (auto abilityIt = abilities.cbegin(); abilityIt != abilities.cend(); ++abilityIt)
					{
						res.abilityUpgrades.push_back(GetAbility(abilityIt->as_object()));
					}
				}),
					MAKE_PARSER("additional_units", { auto& units = val.as_array();
					res.additionalUnits.reserve(units.size());

					for (auto unitIt = units.cbegin(); unitIt != units.cend(); ++unitIt)
					{
						res.additionalUnits.push_back(GetUnit(unitIt->as_object()));
					}
				})
			};

			ResType player;
			ParseObject(player, json, parsers);
			return player;
		}

		MatchHistoryBySeqNumberResult::AbilityUpgrade JSONParser::GetAbility(const web::json::object &json)
		{
			typedef MatchHistoryBySeqNumberResult::AbilityUpgrade ResType;

			DEFINE_PARSER
			{
				MAKE_PARSER("ability",  { res.ability = val.as_number().to_uint32(); }),
					MAKE_PARSER("time",  { res.time = val.as_number().to_uint32(); }),
					MAKE_PARSER("level",  { res.level = val.as_number().to_uint32(); })
			};

			ResType ability;
			ParseObject(ability, json, parsers);
			return ability;
		}

		MatchHistoryBySeqNumberResult::AdditionalUnit JSONParser::GetUnit(const web::json::object &json)
		{
			typedef MatchHistoryBySeqNumberResult::AdditionalUnit ResType;

			DEFINE_PARSER
			{
				MAKE_PARSER("unitname",  { res.unitName = val.as_string(); }),
					MAKE_PARSER("item_0",  { res.item0 = val.as_number().to_uint32(); }),
					MAKE_PARSER("item_1",  { res.item1 = val.as_number().to_uint32(); }),
					MAKE_PARSER("item_2",  { res.item2 = val.as_number().to_uint32(); }),
					MAKE_PARSER("item_3",  { res.item3 = val.as_number().to_uint32(); }),
					MAKE_PARSER("item_4",  { res.item4 = val.as_number().to_uint32(); }),
					MAKE_PARSER("item_5",  { res.item5 = val.as_number().to_uint32(); })
			};

			ResType unit;
			ParseObject(unit, json, parsers);
			return unit;
		}
		
		MatchHistoryBySeqNumberResult::PicksBans JSONParser::GetPickban(const web::json::object &json)
		{
			typedef MatchHistoryBySeqNumberResult::PicksBans ResType;

			DEFINE_PARSER
			{
				MAKE_PARSER("is_pick",  { res.isPick = val.as_bool(); }),
					MAKE_PARSER("hero_id",  { res.heroId = val.as_number().to_uint32(); }),
					MAKE_PARSER("team",  { res.team = val.as_number().to_uint32(); }),
					MAKE_PARSER("order",  { res.order = val.as_number().to_uint32(); })
			};

			ResType pb;
			ParseObject(pb, json, parsers);
			return pb;
		}
	}
}