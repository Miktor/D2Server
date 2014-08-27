#ifndef APIServer_h__
#define APIServer_h__

#include "stdafx.h"
#include <mutex>

using namespace web::http;
using namespace web::http::experimental;
using namespace web::http::experimental::listener;

namespace API
{
	class APIServer
	{
	public:

		typedef std::function<void(http_request)> handlerFunction;

		APIServer();
		~APIServer();

		pplx::task<void> Run();
		void Stop();

	private:

		http_listener listener;

		void handle_get(http_request request);
		void handle_post(http_request request);

		void handle_players(http_request request);
		void handle_matches(http_request request);

		void handle_players_by_id(http_request request);
		void handle_matches_by_id(http_request request);
		
		std::mutex m_mapMutex;
		std::map < utility::string_t, handlerFunction> m_handlers;
	};
}
#endif // APIServer_h__
