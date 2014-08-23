// SteamParser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "JSONParser.h"
#include "DotaAPI.h"
#include "DBInserter.h"
#include <cpprest/http_client.h>

int main()
{
	try{
		web::http::client::http_client_config config;
		config.set_guarantee_order(false);

		SteamParser::Database::DatabaseConfig dbConfig = {
			"shishov.no-ip.org",
			"d2_pick_assist",
			"d2_pick_assist_pwd",
			"d2_pick_assist"
		};


		SteamParser::DotaAPI api(U("77021A84BBD69F1369480175FCC45729"), config, U("570"));

		SteamParser::Database::DBInserter db(dbConfig);

		auto request = api.GetMatchHistoryBySeqNumber(709365483LL);
		auto response = request.get();

		if (response.status_code() != status_codes::OK)
		{
			return response.status_code();
		}
		auto json = response.extract_json().get();
		auto res = SteamParser::JSON::JSONParser::ParseMatchHistoryBySeqNumber(json);

		for (auto it = res.matches.cbegin(); it != res.matches.cend(); ++it)
		{
			db.AddMatch(*it);
		}
	}
	catch (std::exception &e)
	{
		std::cout << "Exception = " << e.what();
		return 1;
	}
	return 0;
}

