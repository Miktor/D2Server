#ifndef Fetcher_h__
#define Fetcher_h__

#include "stdafx.h"
#include <thread>
#include "concurent_queue.h"

namespace SteamParser
{
	class DotaAPI;

	class SteamFetcher
	{
	public:
		SteamFetcher(std::shared_ptr<DotaAPI> api);
		~SteamFetcher();

		SteamFetcher(const SteamFetcher &) = delete;

		void Run(std::shared_ptr<pplx::cancellation_token_source> pCancellationSource, SteamResponseQueuePtr pQueue, std::shared_ptr<std::condition_variable> cv);
		void Stop();
	private:

		void SteamFetcherThreadRun();

		std::shared_ptr<pplx::cancellation_token_source> pCancellationSource;
		std::shared_ptr<std::condition_variable> pCv;
		SteamResponseQueuePtr pQueue;

		std::shared_ptr<DotaAPI> pApi;
		std::unique_ptr<std::thread> m_tSteamFetcher;
	};
}
#endif // Fetcher_h__
