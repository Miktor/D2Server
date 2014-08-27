#ifndef stdafx_h__
#define stdafx_h__

#include <stdint.h>
#include <stdio.h>
#include <map>
#include <memory>
#include <vector>

#include "cpprest/basic_types.h"
#ifdef _MS_WINDOWS
#define NOMINMAX
#include <Windows.h>
#else
# include <sys/time.h>
#endif

#include "cpprest/json.h"
#include "cpprest/http_listener.h"
#include "cpprest/uri.h"
#include "cpprest/asyncrt_utils.h"

#include "concurent_queue.h"

typedef web::json::value SteamResponse;
typedef concurrent_queue<SteamResponse> SteamResponseQueue;
typedef std::shared_ptr<SteamResponseQueue> SteamResponseQueuePtr;

#endif // stdafx_h__
