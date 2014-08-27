#include "stdafx.h"
#include "MatchInserter.h"
#include "JSONParser.h"
#include "DBInserter.h"

namespace SteamParser
{

	MatchInserter::MatchInserter(std::shared_ptr<Database::DBInserter> db)
		: pDb(db)
		, m_tInserter(nullptr)
	{

	}

	MatchInserter::~MatchInserter()
	{

	}

	void MatchInserter::Run(std::shared_ptr<pplx::cancellation_token_source> cancellationSource, SteamResponseQueuePtr queue, std::shared_ptr<std::condition_variable> cv)
	{
		if (m_tInserter)
		{
			throw std::runtime_error("Thread already running!");
		}

		pCv = cv;
		pCancellationSource = cancellationSource;
		pQueue = queue;
		m_tInserter.reset((new std::thread(&MatchInserter::InserterThreadRun, this)));
	}

	void MatchInserter::Stop()
	{
		if (!m_tInserter)
		{
			throw std::runtime_error("Thread not running!");
		}

		pCancellationSource->cancel();
		pCancellationSource.reset();
		pQueue.reset();
		m_tInserter.reset();
		pCv.reset();
	}

	void MatchInserter::InserterThreadRun()
	{
		SteamResponse json;

		auto cancelationToken = pCancellationSource->get_token();
		while (!cancelationToken.is_canceled())
		{		
			if (!pQueue->try_pop(json))
			{
				continue;
			}

			auto res = SteamParser::JSON::JSONParser::ParseMatchHistoryBySeqNumber(json);

			for (auto it = res.matches.cbegin(); it != res.matches.cend(); ++it)
			{
				pDb->AddMatch(*it);
			}
		}
		pCv->notify_one();
	}

}