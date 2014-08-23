#include "stdafx.h"
#include "DotaAPI.h"

#include <cpprest/http_client.h>


namespace SteamParser{

	const utility::string_t StartAtMatchSeqNumParameter = U("&start_at_match_seq_num=");

	DotaAPI::DotaAPI(utility::string_t apiKey, http_client_config config, utility::string_t dotaVersion)
		: m_Client(web::uri(SteamApiUrl), config)
		, m_DotaVersion(dotaVersion)
		, KeyParameter(U("?key=") + apiKey)
	{
	}


	DotaAPI::~DotaAPI()
	{
	}

	Concurrency::task<http_response> DotaAPI::GetMatchHistoryBySeqNumber(const uint64_t &seqNum, Concurrency::cancellation_token token)
	{
		auto requestUrl = DotaMatchPathUrl + m_DotaVersion + GetMatchHistoryBySeqNumberUrl + KeyParameter;
		if (seqNum)
			requestUrl += StartAtMatchSeqNumParameter + std::to_wstring(seqNum);

		return m_Client.request(methods::GET, requestUrl, token);
	}

}