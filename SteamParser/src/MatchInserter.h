#ifndef MatchInserter_h__
#define MatchInserter_h__

#include "stdafx.h"
#include <thread>
#include "concurent_queue.h"

namespace SteamParser
{
	namespace Database
	{
		class DBInserter;
	}

	class MatchInserter
	{
	public:
		MatchInserter(std::shared_ptr<Database::DBInserter> db);
		~MatchInserter();

		MatchInserter(const MatchInserter &) = delete;

		void Run(std::shared_ptr<pplx::cancellation_token_source> cancellationSource, SteamResponseQueuePtr queue, std::shared_ptr<std::condition_variable> cv);
		void Stop();

	private:

		std::shared_ptr<pplx::cancellation_token_source> pCancellationSource;
		std::shared_ptr<std::condition_variable> pCv;
		SteamResponseQueuePtr pQueue;

		std::shared_ptr<Database::DBInserter> pDb;
		std::unique_ptr<std::thread> m_tInserter;

		void InserterThreadRun();
	};
}
#endif // MatchInserter_h__
