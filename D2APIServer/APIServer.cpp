#include "stdafx.h"
#include "APIServer.h"

static const web::http::uri serverAdress = U("http://localhost:8080/api");

namespace API
{
	template<typename Map> typename Map::const_iterator	find_prefix(Map const& map, typename Map::key_type const& key)
	{
		typename Map::const_iterator it = map.upper_bound(key);
		while (it != map.begin())
		{
			--it;
			if (key.substr(0, it->first.size()) == it->first)
				return it;
		}

		return map.end(); // map contains no prefix
	}

	APIServer::APIServer()
		: listener(serverAdress)
	{
		listener.support(methods::GET, std::bind(&APIServer::handle_get, this, std::placeholders::_1));
		listener.support(methods::POST, std::bind(&APIServer::handle_post, this, std::placeholders::_1));

		m_handlers[U("/players/id")] = std::bind(&APIServer::handle_players_by_id, this, std::placeholders::_1);
		m_handlers[U("/matches/id")] = std::bind(&APIServer::handle_matches_by_id, this, std::placeholders::_1);
		m_handlers[U("/players/")] = std::bind(&APIServer::handle_players, this, std::placeholders::_1);
		m_handlers[U("/matches/")] = std::bind(&APIServer::handle_matches, this, std::placeholders::_1);
	}


	APIServer::~APIServer()
	{
	}

	pplx::task<void> APIServer::Run()
	{
		return listener.open();
	}

	void APIServer::Stop()
	{

	}

	void APIServer::handle_get(http_request request)
	{
		TRACE << L"\nhandle GET\n";

		auto path = web::http::uri::decode(request.relative_uri().path());

		m_mapMutex.lock();
		auto it = find_prefix(m_handlers, path);
		bool found = it != m_handlers.end();
		m_mapMutex.unlock();

		if (!found)
		{
			request.reply(status_codes::NotFound, web::json::value(U("Invalid path!")));
			return;
		}

		it->second(request);
	}

	void APIServer::handle_post(http_request request)
	{
		TRACE << L"\nhandle Post\n";

		request.reply(status_codes::NotFound);
	}

	void APIServer::handle_players(http_request request)
	{
		TRACE << L"\nhandle Players\n";

		auto paths = web::http::uri::decode(request.relative_uri().path());

		request.reply(status_codes::NotFound);
	}

	void APIServer::handle_matches(http_request request)
	{
		TRACE << L"\nhandle matches\n";

		auto paths = web::http::uri::decode(request.relative_uri().path());

		request.reply(status_codes::NotFound);
	}

	void APIServer::handle_players_by_id(http_request request)
	{
		TRACE << L"\nhandle players by id\n";

		auto paths = web::http::uri::decode(request.relative_uri().path());

		request.reply(status_codes::NotFound);
	}

	void APIServer::handle_matches_by_id(http_request request)
	{
		TRACE << L"\nhandle matches by id\n";

		auto paths = web::http::uri::decode(request.relative_uri().path());

		request.reply(status_codes::NotFound);
	}
}