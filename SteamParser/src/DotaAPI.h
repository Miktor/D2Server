#ifndef DotaAPI_h__
#define DotaAPI_h__

#include "stdafx.h"
#include <cpprest/http_client.h>

using namespace web;
using namespace web::http;
using namespace web::http::client;

namespace SteamParser
{
	class DotaAPI
	{
	public:
		const utility::char_t*	SteamApiUrl = U("https://api.steampowered.com");
		const utility::string_t DotaMatchPathUrl = U("/IDOTA2Match_");
		const utility::string_t GetMatchHistoryBySeqNumberUrl = U("/GetMatchHistoryBySequenceNum/v1/");

		DotaAPI(utility::string_t apiKey, http_client_config config, utility::string_t dotaVersion);
		~DotaAPI();

		Concurrency::task<http_response> GetMatchHistoryBySeqNumber(const uint64_t &seqNum = 0, Concurrency::cancellation_token token = Concurrency::cancellation_token::none());

	private:

		const utility::string_t KeyParameter;
		const utility::string_t m_DotaVersion;

		http_client m_Client;

	};

}

#endif // DotaAPI_h__
