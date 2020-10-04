#ifndef _WEBSOCKET_SERVER
#define _WEBSOCKET_SERVER

//We need to define this when using the Asio library without Boost
#define ASIO_STANDALONE

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <json/json.h>

#include <functional>
#include <string>
#include <vector>
#include <mutex>
#include <map>

using std::string;
using std::vector;
using std::map;

typedef websocketpp::server<websocketpp::config::asio> WebsocketEndpoint;
typedef websocketpp::connection_hdl ClientConnection;
typedef void(*callback_function)(char *); // type for conciseness
typedef unsigned char       BYTE;

//The port number the WebSocket server listens on
#define PORT_NUMBER 10522

class WebsocketServer
{
private:

	enum ServerType { ST_UNKNOWN, ST_STANDALONE, ST_INTEGRATED };

	enum StatusMess {
		SM_UNKNOWNCOMMAND,
		SM_UNKNOWN,
		SM_ERROR
	};

	enum Commands { 
		C_ATR,
		C_VIEW_CERT,
		C_AUTH,
		C_SIGN
	};

	map<int, string> messages;
	map<int, string> commands;
	string lastCardError;

	StatusMess  lastPollStatus = SM_UNKNOWN;

	ServerType type;

	string lastError;
	string atr = "";

	int16_t port;

	int timer = 0;
	int isAuthenticated = 0;

	bool permanentConnection;

	bool messageParse(ClientConnection conn, string message);

	callback_function rcv_callback = NULL;

	public:
		
		WebsocketServer(int16_t port = PORT_NUMBER, ServerType type = ST_STANDALONE);
		void run();
		
		//Returns the number of currently connected clients
		size_t numConnections();
		
		//Registers a callback for when a client connects
		template <typename CallbackTy>
		void connect(CallbackTy handler)
		{
			//Make sure we only access the handlers list from the networking thread
			this->eventLoop.post([this, handler]() {
				this->connectHandlers.push_back(handler);
			});
		}
		
		//Registers a callback for when a client disconnects
		template <typename CallbackTy>
		void disconnect(CallbackTy handler)
		{
			//Make sure we only access the handlers list from the networking thread
			this->eventLoop.post([this, handler]() {
				this->disconnectHandlers.push_back(handler);
			});
		}
		
		//Registers a callback for when a particular type of message is received
		template <typename CallbackTy>
		void message(const string& messageType, CallbackTy handler)
		{
			//Make sure we only access the handlers list from the networking thread
			this->eventLoop.post([this, messageType, handler]() {
				this->messageHandlers[messageType].push_back(handler);
			});
		}
		
		//Sends a message to an individual client
		//(Note: the data transmission will take place on the thread that called WebsocketServer::run())
		void sendMessage(ClientConnection conn, const string& messageType, const Json::Value& arguments);
		
		//Sends a message to all connected clients
		//(Note: the data transmission will take place on the thread that called WebsocketServer::run())
		void broadcastMessage(const string& messageType, const Json::Value& arguments);

		void set_rcv_callback(callback_function f) { rcv_callback = f; }
		
	protected:
		static Json::Value parseJson(const string& json);
		static string stringifyJson(const Json::Value& val);
		
		void onOpen(ClientConnection conn);
		void onClose(ClientConnection conn);
		void onMessage(ClientConnection conn, WebsocketEndpoint::message_ptr msg);
		
		asio::io_service eventLoop;
		WebsocketEndpoint endpoint;
		vector<ClientConnection> openConnections;
		std::mutex connectionListMutex;
		
		vector<std::function<void(ClientConnection)>> connectHandlers;
		vector<std::function<void(ClientConnection)>> disconnectHandlers;
		map<string, vector<std::function<void(ClientConnection, const Json::Value&)>>> messageHandlers;
};

#endif
