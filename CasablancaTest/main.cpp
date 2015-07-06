// C++ Casablanca REST SDK - Listener example
// http://mariusbancila.ro/blog/tag/casablanca/
// By Paul Hindt - 07/04/2015

#include <iostream>
#include <map>
#include <exception>
#include <cpprest\http_listener.h>
#include <cpprest\json.h>

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

#define TRACE(msg) std::wcout << msg; // TRACE(L"handle get...\n");
#define TRACE_ACTION(a, k, v) std::wcout << a << L" (" << k << L", " << v << L")\n" // TRACE_ACTION(L"deleted", pos->first, pos->second);

enum web_actions {
	BROADCAST,
	RECORD,
	REPLAY,
	VIRTUAL_CAMERA
} kWebActions;

void handle_get(http_request req)
{
	auto query_vars = uri::split_query(req.request_uri().query()); // query string variables in std::map<utility::string_t, utility::string_t>
	//std::map<utility::string_t, utility::string_t>::iterator qi; // query string iterator

	// old style STL container iteration
	//for (qi = query_vars.begin(); qi != query_vars.end(); qi++) { }

	// c++11 container iteration
	for (auto qi : query_vars) {
		if (qi.first == L"broadcast") {
			kWebActions = web_actions::BROADCAST;
			if (qi.second == L"start") { std::wcout << "Starting all broadcasts...\n"; /* wirecast.broadcast.start(); */ }
			else if (qi.second == L"stop") { std::wcout << "Stopping all broadcasts...\n"; /* wirecast.broadcast.stop(); */ }
		}
		if (qi.first == L"record") {
			kWebActions = web_actions::RECORD;
		}
		if (qi.first == L"replay") {
			kWebActions = web_actions::REPLAY;
		}
		if (qi.first == L"vcam") {
			kWebActions = web_actions::VIRTUAL_CAMERA;
		}
	}

	// json data returned
	json::value json_data;
	json_data[0] = json::value("json test string");
	json_data[1] = json::value(42);

	TRACE(L"Handled GET...\n");
	//req.reply(status_codes::OK, "GET request...", "text/plain; charset=utf-8");
	req.reply(status_codes::OK, json_data);
}

// todo: learn how to handle POST requests
void handle_post(http_request req) {  }

int main()
{
	http_listener listener(L"http://localhost:9999/rest/api");
	listener.support(methods::GET, handle_get);
	listener.support(methods::POST, handle_post);

	try {
		listener
			.open()
			.then([&listener](){ TRACE(L"Starting listener...\n"); })
			.wait();
		while (true);
	}
	catch (std::exception const &e)
	{
		std::wcout << e.what() << std::endl;
	}

	return 0;
}