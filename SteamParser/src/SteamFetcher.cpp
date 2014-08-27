#include "stdafx.h"
#include "SteamFetcher.h"
#include "DotaAPI.h"

namespace SteamParser
{
	SteamFetcher::SteamFetcher(std::shared_ptr<DotaAPI> api)
		: pApi(api)
		, m_tSteamFetcher(nullptr)
	{
	}


	SteamFetcher::~SteamFetcher()
	{
	}

	void SteamFetcher::Run(std::shared_ptr<pplx::cancellation_token_source> cancellationSource, SteamResponseQueuePtr queue, std::shared_ptr<std::condition_variable> cv)
	{	
		if (m_tSteamFetcher)
		{
			throw std::runtime_error("Thread already running!");
		}

		pCv = cv;
		pCancellationSource = cancellationSource;
		pQueue = queue;
		m_tSteamFetcher.reset(new std::thread(&SteamFetcher::SteamFetcherThreadRun, this));
	}

	void SteamFetcher::Stop()
	{
		if (!m_tSteamFetcher)
		{
			throw std::runtime_error("Thread not running!");
		}

		pCancellationSource->cancel();
		pCancellationSource.reset();
		pQueue.reset();
		m_tSteamFetcher.reset();
		pCv.reset();
	}

	void SteamFetcher::SteamFetcherThreadRun()
	{
		auto cancelationToken = pCancellationSource->get_token();
		while (!cancelationToken.is_canceled())
		{
			auto response = pApi->GetMatchHistoryBySeqNumber(709365483LL).get();

			if (response.status_code() != status_codes::OK)
			{
				continue;
			}

			pQueue->push(response.extract_json().get());
		}
		pCv->notify_one();
	}

}