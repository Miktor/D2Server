// D2APIServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <cpprest/http_server.h>
#include <cpprest/http_server_api.h>

#if WIN32
#include <cpprest/http_windows_server.h>
#define SERVER_CLASS http_windows_server
#else
#include <cpprest/http_linux_server.h>
#define SERVER_CLASS http_linux_server
#endif // WIN32


namespace details = web::http::experimental::details;

int main()
{
	auto server = std::make_unique<details::SERVER_CLASS>();
	details::http_server_api::register_server_api(server)
	return 0;
}

