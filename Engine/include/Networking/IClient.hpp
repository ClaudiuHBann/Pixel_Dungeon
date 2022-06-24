#pragma once

#ifdef _WIN32

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00
#endif // _WIN32_WINNT

#endif // _WIN32

#ifndef ASIO_STANDALONE
#define ASIO_STANDALONE
#endif // ASIO_STANDALONE

#include <asio/ip/tcp.hpp>

#include <string>

class IClient {
public:
	IClient() {}
	virtual ~IClient() {}

	virtual inline void Set(const std::string& host, const uint16_t port) = 0;

	virtual inline void Connect() = 0;
	virtual inline void ConnectAsync() {}
	virtual inline void Disconnect() = 0;

	virtual inline void Send(const std::string& data) = 0;
	virtual inline void SendAsync(const std::string& data) {}

	virtual inline std::string Receive(const uint32_t bytesToRead = 0) = 0;
	virtual inline void ReceiveAsync(const uint32_t bytesToRead = 0) {}

	inline asio::error_code GetErrorCode() const {
		return ec;
	}

protected:
	virtual void OnConnect(const asio::ip::tcp::endpoint& endpoint) {}
	virtual void OnSend(const std::string& data) {}
	virtual void OnReceive(const std::string& data) {}
	virtual void OnDisconnect() {}

	asio::error_code ec;
};