#ifndef Parser_h__
#define Parser_h__

#include "stdafx.h"
#include <cpprest/json.h>
#include "Types.h"

using namespace web;

namespace SteamParser{

	class DotaAPI;

	namespace JSON
	{		

		class JSONParser
		{
		private:
			JSONParser() {}
			~JSONParser() {}

		public:
			static MatchHistoryBySeqNumberResult ParseMatchHistoryBySeqNumber(web::json::value &json);

		private:

			template<class T>
			static void ParseObject(T& object, const web::json::object &json, std::map<utility::string_t, std::function<void(T&, const web::json::value&)>> parsers);

			static MatchHistoryBySeqNumberResult::Match GetMatch(const web::json::object &json);
			static MatchHistoryBySeqNumberResult::Player GetPlayer(const web::json::object &json);

			static MatchHistoryBySeqNumberResult::AbilityUpgrade GetAbility(const web::json::object &json);
			static MatchHistoryBySeqNumberResult::AdditionalUnit GetUnit(const web::json::object &json);
			static MatchHistoryBySeqNumberResult::PicksBans GetPickban(const web::json::object &json);
		};
	}
}

#endif // Parser_h__