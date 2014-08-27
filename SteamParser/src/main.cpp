// SteamParser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "JSONParser.h"
#include "DotaAPI.h"
#include "DBInserter.h"
#include "SteamFetcher.h"
#include "MatchInserter.h"
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
		
		auto pApi = std::make_shared<SteamParser::DotaAPI>(U("77021A84BBD69F1369480175FCC45729"), config, U("570"));
		auto pDb = std::make_shared<SteamParser::Database::DBInserter>(dbConfig);

		SteamParser::SteamFetcher fetcher(pApi);
		SteamParser::MatchInserter inserter(pDb);

		auto cvFetcher = std::make_shared<std::condition_variable>();
		auto cvInserter = std::make_shared<std::condition_variable>();

		auto pCancelation = std::make_shared<pplx::cancellation_token_source>();
		auto pQueue = std::make_shared<SteamResponseQueue>();

		fetcher.Run(pCancelation, pQueue, cvFetcher);
		inserter.Run(pCancelation, pQueue, cvInserter);

		std::mutex mtx;
		std::unique_lock<std::mutex> lck(mtx);
		cvFetcher->wait(lck);
		cvInserter->wait(lck);
	}
	catch (std::exception &e)
	{
		std::cout << "Exception = " << e.what();
		return 1;
	}
	return 0;
}

