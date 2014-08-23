#ifndef DBInserter_h__
#define DBInserter_h__

#include "stdafx.h"

#include <cpprest/asyncrt_utils.h>

#include <cppconn/sqlstring.h>
#include <cppconn/prepared_statement.h>

#include "Types.h"

namespace sql
{
	class Driver;
	class Connection;
	class PreparedStatement;
}

namespace SteamParser
{
	namespace Database
	{
		struct DatabaseConfig
		{
			sql::SQLString url;
			sql::SQLString user;
			sql::SQLString password;

			sql::SQLString dbName;
		};

		class DBInserter
		{
		public:
			DBInserter(const DatabaseConfig &config);
			~DBInserter();

			void AddMatch(const MatchHistoryBySeqNumberResult::Match &match);

		private:
			
			void addUnits(const uint64_t &matchId, const uint64_t &accountId, const std::vector<MatchHistoryBySeqNumberResult::AdditionalUnit> &upgrades);
			void addSkillBuild(const uint64_t &skillBuildId, const std::vector<MatchHistoryBySeqNumberResult::AbilityUpgrade> &abilities);
			void addPlayer(const uint64_t &matchId, const MatchHistoryBySeqNumberResult::Player &player);
			void addTeam(const MatchHistoryBySeqNumberResult::Match &match);
			void addPicks(const uint64_t &matchId, const std::vector<MatchHistoryBySeqNumberResult::PicksBans> &picksBans);
			void addMatchData(const MatchHistoryBySeqNumberResult::Match &match);

			class Statements
			{
			public:
				Statements(std::shared_ptr<sql::Connection> pConnection);
				Statements() {}

				~Statements() {}

				std::shared_ptr<sql::PreparedStatement> m_pAddMatch;
				std::shared_ptr<sql::PreparedStatement> m_pAddPlayer;
				std::shared_ptr<sql::PreparedStatement> m_pAddTeam;
				std::shared_ptr<sql::PreparedStatement> m_pAddSkillBuild;
				std::shared_ptr<sql::PreparedStatement> m_pAddCaptain;
				std::shared_ptr<sql::PreparedStatement> m_pAddPickBan;
				std::shared_ptr<sql::PreparedStatement> m_pAddUnit;
				std::shared_ptr<sql::PreparedStatement> m_pGetLastMatchSeqNum;
			};

			sql::Driver *m_pDriver;
			std::shared_ptr<sql::Connection> m_pConnection;
			std::shared_ptr<Statements> m_pStatements;
		};
	}
}
#endif // DBInserter_h__
