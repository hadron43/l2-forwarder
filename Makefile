default:
	g++ client.cpp -pthread -o build/client
	g++ server.cpp -pthread -o build/server