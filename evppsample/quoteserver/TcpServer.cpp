//an quote of the day server
#include <string>
#include <iostream>
#include <fstream>
#include <function>
#include "../../array_list.h"
#include "evpp/libevent.h"
#include "evpp/event_watcher.h"
#include "evpp/event_loop.h"
#include "evpp/event_loop_thread.h"
#include "evpp/tcp_server.h"
#include "evpp/buffer.h"
#include "evpp/tcp_callbacks.h"
#include "evpp/tcp_conn.h"

constexpr bool OpenSuccess = true; 
class App;


class Qod
{
public:
	Qod() = default;
	~Qod() = default;

	bool OpenQuote(const string& filename)
	{
		std::ifstream file;
		file.open(filename);

		if (OpenSuccess != file.is_open())
		{
			std::cout << " bull shit, open file Not good"<< std::endl;
			return false;
		}

		std::string line;
		std::string running = "";

		while (file.getline(line))
		{
			if (line != "%")
			{
				running = running + line + "\n";
			}
			else
			{
				QuoteArray.push_back(running);
				running = "";
			}
		}
		file.close()
		return OpenSuccess;
	}
	string GetRandomQuote()
	{
		int random = rand() % QuoteArray.size();
		return QuoteArray[random];
	}
private:
	VihoStructures::ArrayList<string> QuoteArray;
};
class App
{
	void OnMessage(const evpp::TCPConnPtr& conn,
		evpp::Buffer* msg) {
		std::string s = msg->NextAllString();
		LOG_INFO << "Received a message [" << s << "]";
		if (s == "quit" || s == "exit") {
			conn->Close();
		}
		else
		{
			conn->Send(App::GetInstance()->GetRandomQuote());
		}
	}


	void OnConnection(const evpp::TCPConnPtr& conn) {
		if (conn->IsConnected()) {
			LOG_INFO << "Accept a new connection from " << conn->remote_addr();
		}
		else {
			LOG_INFO << "Disconnected from " << conn->remote_addr();
		}
	}
// 	void SetMessageCallback(MessageCallback callback)
// 	{
// 		server.SetMessageCallback(&callback);
// 	}
// 
// 	void SetConnectionCallback(const ConnectionCallback& callback)
// 	{
// 		server.SetConnectionCallback(&callback);
// 	}
	bool init()
	{
		server.Init();
		server.Start();
	}
	bool run()
	{
		loop.Run();
	}
	string GetRandomQuote()
	{
		return Quotes.GetRandomQuote();
	}

public:
	static App* GetInstance(const string& Addr, const String& Filename)
	{
		static App instance(Addr, Filename);
		return &instance;
	}
private:
	App(const string& Addr = "", const String& Filename = "");
	{
		quote.OpenQuote(filename);
		
		evpp::TCPServer server(&loop, Addr, "TCPEcho", 0);
	}
	~App()
	{
		loop.RunAfter(evpp::Duration(1.0), std::bind(&evpp::TCPClient::Disconnect, client));
	};

private:

	Qod Quotes;
	evpp::EventLoop loop;
	evpp::TCPServer server;
}

int main(int argc, char* argv[]) {
	std::string port = "9099";
	if (argc == 2) {
		port = argv[1];
	}
	std::string addr = std::string("0.0.0.0:") + port;
	auto appins = App::GetInstance(addr, "wisdom.txt");
	std::function<void(const evpp::TCPConnPtr&, evpp::Buffer*)> messageFnc = std::bind(&App::OnMessage, App, std::placeholders::_1, std::placeholders::_2);
	appins->SetMessageCallback(&OnMessage);
	std::function<void(const TCPConnPtr&)> connectFunc = std::bind(&App::OnMessage, App, std::placeholders::_1);
	appins->SetConnectionCallback(&OnConnection);
	
	appins->Start();
	appins->Run();
	return 0;
}




#ifdef _WIN32
#include "../../winmain-inl.h"
#endif