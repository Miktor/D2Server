// D2APIServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "APIServer.h"
//namespace details = web::http::experimental::details;

int main()
{
	try
	{
		API::APIServer server;

		{
			server.Run()
				.then([&server]() { TRACE << L"\nstarting to listen\n"; })
				.wait();
		} while (true);

		return 0;
	}
	catch (std::exception &e)
	{
		TRACE << "Exception = " << e.what();
		return 1;
	}
}

