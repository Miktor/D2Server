#ifndef APIServer_h__
#define APIServer_h__

#include "stdafx.h"

namespace API
{
	class APIServer
	{
	public:
		APIServer();
		~APIServer();

		void Run();
		void Stop();

	};
}
#endif // APIServer_h__
