#include "stdafx.h"
#include "DBInserter.h"

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

static const sql::SQLString queryAddMatch = "INSERT INTO matches (season, radiant_win, duration, start_time, match_id, match_seq_num, cluster, first_blood_time, lobby_type, human_players, leagueid, positive_votes, negative_votes, game_mode, tower_status_radiant, tower_status_dire, barracks_status_radiant, barracks_status_dire) " 
"VALUES( ?, ?, ?, FROM_UNIXTIME( ? ), ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ? )";
static const sql::SQLString queryAddPlayer = "INSERT INTO match_players (match_id, account_id, hero_id, item_0, item_1, item_2, item_3, item_4, item_5, kills, deaths, assists, leaver_status, gold, last_hits, denies, gold_per_min, xp_per_min, gold_spent, hero_damage, tower_damage, hero_healing, level, skill_build, player_slot) " 
"VALUES( ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
static const sql::SQLString queryAddTeam = "INSERT INTO match_teams (match_id, team_id, name, logo, team_complete, guild_id, guild_name, guild_logo, radiant) VALUES( ?, ?, ?, ?, ?, ?, ?, ?, ?)";
static const sql::SQLString queryAddSkillBuilds = "INSERT INTO player_skill_builds (build_id, level, ability, time) VALUES( ?, ?, ?, FROM_UNIXTIME( ? ) )";
static const sql::SQLString queryAddCaptain = "INSERT INTO match_captains (match_id, captain, radiant) VALUES( ?, ?, ?)";
static const sql::SQLString queryAddPickBans  = "INSERT INTO match_picks_bans (match_id, `order`, is_pick, hero_id, team) VALUES( ?, ?, ?, ?, ? )";
static const sql::SQLString queryAddUnits  = "INSERT INTO match_additional_units (match_id, account_id, unitname, item_0, item_1, item_2, item_3, item_4, item_5) VALUES( ?, ?, ?, ?, ?, ?, ?, ?, ?)";
static const sql::SQLString queryNeedUpdateAccounts = "SELECT account_id FROM registred_users";
static const sql::SQLString queryNeedMatch = "SELECT match_id FROM matches WHERE match_id = ?";
static const sql::SQLString queryLastMatchSeqNum = "SELECT match_seq_num FROM matches ORDER BY match_seq_num DESC LIMIT 1";

namespace SteamParser
{
	namespace Database
	{
		DBInserter::Statements::Statements(std::shared_ptr<sql::Connection> pConnection)
		{
			m_pAddMatch.reset(pConnection->prepareStatement(queryAddMatch));
			m_pAddPlayer.reset(pConnection->prepareStatement(queryAddPlayer));
			m_pAddTeam.reset(pConnection->prepareStatement(queryAddTeam));
			m_pAddSkillBuild.reset(pConnection->prepareStatement(queryAddSkillBuilds));
			m_pAddCaptain.reset(pConnection->prepareStatement(queryAddCaptain));
			m_pAddPickBan.reset(pConnection->prepareStatement(queryAddPickBans));
			m_pAddUnit.reset(pConnection->prepareStatement(queryAddUnits));
			m_pGetLastMatchSeqNum.reset(pConnection->prepareStatement(queryLastMatchSeqNum));
		}

		DBInserter::DBInserter(const DatabaseConfig &config)
			: m_pDriver(get_driver_instance())
		{
			m_pConnection.reset(m_pDriver->connect(config.url, config.user, config.password));
			m_pConnection->setSchema(config.dbName); 

			m_pStatements.reset(new Statements(m_pConnection));
		}
		
		DBInserter::~DBInserter()
		{
		}

		void DBInserter::addUnits(const uint64_t &matchId, const uint64_t &accountId, const std::vector<MatchHistoryBySeqNumberResult::AdditionalUnit> &upgrades)
		{
			for (auto it = upgrades.cbegin(); it != upgrades.cend(); ++it)
			{
				const auto& upgrade = *it;
				m_pStatements->m_pAddUnit->setUInt64(1, matchId);
				m_pStatements->m_pAddUnit->setUInt64(2, accountId);
				m_pStatements->m_pAddUnit->setString(3, sql::SQLString(utility::conversions::to_utf8string(upgrade.unitName)));
				m_pStatements->m_pAddUnit->setUInt(4, upgrade.item0);
				m_pStatements->m_pAddUnit->setUInt(5, upgrade.item1);
				m_pStatements->m_pAddUnit->setUInt(6, upgrade.item2);
				m_pStatements->m_pAddUnit->setUInt(7, upgrade.item3);
				m_pStatements->m_pAddUnit->setUInt(8, upgrade.item4);
				m_pStatements->m_pAddUnit->setUInt(9, upgrade.item5);
				m_pStatements->m_pAddUnit->executeUpdate();
			}
		}

		void DBInserter::addSkillBuild(const uint64_t &skillBuildId, const std::vector<MatchHistoryBySeqNumberResult::AbilityUpgrade> &abilities)
		{
			for (auto it = abilities.cbegin(); it != abilities.cend(); ++it)
			{
				const auto& ability = *it;
				m_pStatements->m_pAddSkillBuild->setUInt64(1, skillBuildId);
				m_pStatements->m_pAddSkillBuild->setUInt(2, ability.level);
				m_pStatements->m_pAddSkillBuild->setUInt(3, ability.ability);
				m_pStatements->m_pAddSkillBuild->setUInt(4, ability.time);

				std::shared_ptr<sql::ResultSet> res(m_pStatements->m_pAddUnit->executeQuery());
				if (res->next())
				{

				}
			}
		}

		void DBInserter::addPlayer(const uint64_t &matchId, const MatchHistoryBySeqNumberResult::Player &player)
		{
			uint64_t skillBuildId = 0;

			m_pStatements->m_pAddPlayer->setUInt64(1, matchId);
			m_pStatements->m_pAddPlayer->setUInt(2, player.accountId);
			m_pStatements->m_pAddPlayer->setUInt(3, player.heroId);
			m_pStatements->m_pAddPlayer->setUInt(4, player.item0);
			m_pStatements->m_pAddPlayer->setUInt(5, player.item1);
			m_pStatements->m_pAddPlayer->setUInt(6, player.item2);
			m_pStatements->m_pAddPlayer->setUInt(7, player.item3);
			m_pStatements->m_pAddPlayer->setUInt(8, player.item4);
			m_pStatements->m_pAddPlayer->setUInt(9, player.item5);
			m_pStatements->m_pAddPlayer->setUInt(10, player.kills);
			m_pStatements->m_pAddPlayer->setUInt(11, player.deaths);
			m_pStatements->m_pAddPlayer->setUInt(12, player.assists);
			m_pStatements->m_pAddPlayer->setUInt(13, player.leaverStatus);
			m_pStatements->m_pAddPlayer->setUInt(14, player.gold);
			m_pStatements->m_pAddPlayer->setUInt(15, player.lastHits);
			m_pStatements->m_pAddPlayer->setUInt(16, player.denies);
			m_pStatements->m_pAddPlayer->setUInt(17, player.gpm);
			m_pStatements->m_pAddPlayer->setUInt(18, player.xpm);
			m_pStatements->m_pAddPlayer->setUInt(19, player.goldSpent);
			m_pStatements->m_pAddPlayer->setUInt(20, player.heroDamage);
			m_pStatements->m_pAddPlayer->setUInt(21, player.towerDamage);
			m_pStatements->m_pAddPlayer->setUInt(22, player.heroHealing);
			m_pStatements->m_pAddPlayer->setUInt(23, player.level);
			m_pStatements->m_pAddPlayer->setUInt64(24, skillBuildId);
			m_pStatements->m_pAddPlayer->setUInt(25, player.playerSlot);

			m_pStatements->m_pAddPlayer->executeUpdate();

			addSkillBuild(skillBuildId, player.abilityUpgrades);
			addUnits(matchId, player.accountId, player.additionalUnits);
		}

		void DBInserter::addTeam(const MatchHistoryBySeqNumberResult::Match &match)
		{
			if (match.radiantCaptain)
			{
				m_pStatements->m_pAddCaptain->setInt64(1, match.matchId);
				m_pStatements->m_pAddCaptain->setUInt(2, match.radiantCaptain);
				m_pStatements->m_pAddCaptain->setBoolean(3, true);

				m_pStatements->m_pAddCaptain->executeUpdate();
			}

			if (match.direCaptain)
			{
				m_pStatements->m_pAddCaptain->setInt64(1, match.matchId);
				m_pStatements->m_pAddCaptain->setUInt(2, match.direCaptain);
				m_pStatements->m_pAddCaptain->setBoolean(3, false);

				m_pStatements->m_pAddCaptain->executeUpdate();
			}

			if (match.radiantTeamid)
			{
				auto rn = utility::conversions::to_utf8string(match.radiantName);
				auto rgn = utility::conversions::to_utf8string(match.radiantGuildName);

				m_pStatements->m_pAddTeam->setInt64(1, match.matchId);
				m_pStatements->m_pAddTeam->setUInt(2, match.radiantTeamid);
				m_pStatements->m_pAddTeam->setString(3, rn);
				m_pStatements->m_pAddTeam->setUInt(4, match.radiantLogo);
				m_pStatements->m_pAddTeam->setUInt(5, match.radiantTeamComplete);
				m_pStatements->m_pAddTeam->setUInt(6, match.radiantGuildId);
				m_pStatements->m_pAddTeam->setString(7, rgn);
				m_pStatements->m_pAddTeam->setUInt(8, match.radiantGuildLogo);
				m_pStatements->m_pAddTeam->setBoolean(9, true);

				m_pStatements->m_pAddTeam->executeUpdate();
			}

			if (match.direTeamid)
			{
				auto dn = utility::conversions::to_utf8string(match.direName);
				auto dgn = utility::conversions::to_utf8string(match.direGuildName);

				m_pStatements->m_pAddTeam->setInt64(1, match.matchId);
				m_pStatements->m_pAddTeam->setUInt(2, match.direTeamid);
				m_pStatements->m_pAddTeam->setString(3, dn);
				m_pStatements->m_pAddTeam->setUInt(4, match.direLogo);
				m_pStatements->m_pAddTeam->setUInt(5, match.direTeamComplete);
				m_pStatements->m_pAddTeam->setUInt(6, match.direGuildId);
				m_pStatements->m_pAddTeam->setString(7, dgn);
				m_pStatements->m_pAddTeam->setUInt(8, match.direGuildLogo);
				m_pStatements->m_pAddTeam->setBoolean(9, false);

				m_pStatements->m_pAddTeam->executeUpdate();
			}
		}

		void DBInserter::addPicks(const uint64_t &matchId, const std::vector<MatchHistoryBySeqNumberResult::PicksBans> &picksBans)
		{
			for (auto it = picksBans.cbegin(); it != picksBans.cend(); ++it)
			{
				auto& pb = *it;
				m_pStatements->m_pAddPickBan->setUInt64(1, matchId);
				m_pStatements->m_pAddPickBan->setUInt(2, pb.order);
				m_pStatements->m_pAddPickBan->setBoolean(3, pb.isPick);
				m_pStatements->m_pAddPickBan->setUInt(4, pb.heroId);
				m_pStatements->m_pAddPickBan->setUInt(5, pb.team);

				auto res = m_pStatements->m_pAddPickBan->executeQuery();
				if (res->next())
				{

				}
			}
		}

		void DBInserter::addMatchData(const MatchHistoryBySeqNumberResult::Match &match)
		{
			m_pStatements->m_pAddMatch->setUInt64(1, match.season);
			m_pStatements->m_pAddMatch->setBoolean(2, match.radiantWin);
			m_pStatements->m_pAddMatch->setUInt(3, match.durration);
			m_pStatements->m_pAddMatch->setUInt(4, match.startTime);
			m_pStatements->m_pAddMatch->setUInt64(5, match.matchId);
			m_pStatements->m_pAddMatch->setUInt64(6, match.matchSeq);
			m_pStatements->m_pAddMatch->setUInt(7, match.cluster);
			m_pStatements->m_pAddMatch->setUInt(8, match.firstbloodTime);
			m_pStatements->m_pAddMatch->setUInt(9, match.lobbyType);
			m_pStatements->m_pAddMatch->setUInt(10, match.humanPlayers);
			m_pStatements->m_pAddMatch->setUInt(11, match.leagueId);
			m_pStatements->m_pAddMatch->setUInt(12, match.positiveVotes);
			m_pStatements->m_pAddMatch->setUInt(13, match.negativeVotes);
			m_pStatements->m_pAddMatch->setUInt(14, match.gameMode);
			m_pStatements->m_pAddMatch->setUInt(15, match.towerStatusRadiant);
			m_pStatements->m_pAddMatch->setUInt(16, match.towerStatusDire);
			m_pStatements->m_pAddMatch->setUInt(17, match.barracksStatusRadiant);
			m_pStatements->m_pAddMatch->setUInt(18, match.barracksStatusDire);

			addPicks(match.matchId, match.picksBans);
			addTeam(match);
		}

		void DBInserter::AddMatch(const MatchHistoryBySeqNumberResult::Match &match)
		{
			addMatchData(match);

			for (auto it = match.players.cbegin(); it != match.players.cend(); ++it)
			{
				addPlayer(match.matchId, *it);
			}
		}

	}
}
