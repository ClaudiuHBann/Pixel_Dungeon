#pragma once

#ifdef _WIN32

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00
#endif // _WIN32_WINNT

#endif // _WIN32

#ifndef ASIO_STANDALONE
#define ASIO_STANDALONE
#endif // ASIO_STANDALONE

#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

#include "IClient.hpp"

#include "Utility/Miscellaneous.hpp"

class ClientTCP: IClient {
public:
	ClientTCP(asio::io_context& ios);
	ClientTCP(asio::io_context& ios, const std::string& host, const uint16_t port);

	~ClientTCP();

	inline void Set(const std::string& host, const uint16_t port) override {
		auto address = asio::ip::make_address(host, ec);
		if(ec) {
			Miscellaneous::PrintError("An address from host '" + host + "' could not be created! ASIO_Error: " + ec.message() + '.');
			return;
		}

		endpoint.address(address);
		endpoint.port(port);
	}

	inline void Connect() override {
		socket.connect(endpoint, ec);
		if(ec) {
			Miscellaneous::PrintError("The TCP client could not be connected to the server '" + endpoint.address().to_string() + ':' + std::to_string(endpoint.port()) + "'! ASIO_Error: " + ec.message() + '.');
		} else {
			OnConnect(endpoint);
		}
	}

	inline void ConnectAsync() override {
		auto endpoints = asio::ip::tcp::resolver(ios).resolve(endpoint, ec);
		if(ec) {
			Miscellaneous::PrintError("The server '" + endpoint.address().to_string() + ':' + std::to_string(endpoint.port()) + "' could not be resolved! ASIO_Error: " + ec.message() + '.');
		} else {
			asio::async_connect(socket, endpoints, [this] (std::error_code ec, asio::ip::tcp::endpoint ep) {
				if(ec) {
					Miscellaneous::PrintError("The TCP client could not be connected to the server '" + endpoint.address().to_string() + ':' + std::to_string(endpoint.port()) + "'! ASIO_Error: " + ec.message() + '.');
				} else {
					OnConnect(ep);
				}
			});
		}
	}

	inline void Disconnect() override {
		if(!socket.is_open()) {
			Miscellaneous::PrintError("The TCP client was not created successfully!");
			return;
		}

		socket.shutdown(socket.shutdown_both, ec);
		if(ec) {
			Miscellaneous::PrintError("The TCP client could not shutdown communications! ASIO_Error: " + ec.message() + '.');
		}

		socket.close(ec);
		if(ec) {
			Miscellaneous::PrintError("The TCP client could not be closed! ASIO_Error: " + ec.message() + '.');
		}

		OnDisconnect();
	}

	inline void Send(const std::string& data) override {
		if(!socket.is_open()) {
			Miscellaneous::PrintError("The TCP client was not created successfully!");
			return;
		}

		socket.wait(socket.wait_write, ec);
		if(ec) {
			Miscellaneous::PrintError("The TCP client could not wait for upcoming data sending! ASIO_Error: " + ec.message() + '.');
			return;
		}

		socket.write_some(asio::buffer(data), ec);
		if(ec) {
			Miscellaneous::PrintError("The TCP client could not send data! ASIO_Error: " + ec.message() + '.');
		} else {
			OnSend(data);
		}
	}

	inline void SendAsync(const std::string& data) override {
		throw std::logic_error("The method 'ClientTCP::SendAsync' has not yet been implemented!");

		/*
		if(!socket.is_open()) {
			Miscellaneous::PrintError("The TCP client was not created successfully!");
			return;
		}

		socket.async_wait(socket.wait_write, [this, data] (const asio::error_code& ec) {
			if(ec) {
				Miscellaneous::PrintError("The TCP client could not wait for upcoming data sending! ASIO_Error: " + ec.message() + '.');
				return;
			}

			socket.async_write_some(asio::buffer(data), [this, data] (const asio::error_code& ec, size_t bytesTransferred) {
				if(ec) {
					Miscellaneous::PrintError("The TCP client could not send data! ASIO_Error: " + ec.message() + '.');
				} else {
					OnSend(data.substr(0, bytesTransferred));
				}
			});
		});
		*/
	}

	inline std::string Receive(const uint32_t bytesToRead = 0) override {
		std::string data("");

		if(!socket.is_open()) {
			Miscellaneous::PrintError("The TCP client was not created successfully!");
			return data;
		}

		socket.wait(socket.wait_read, ec);
		if(ec) {
			Miscellaneous::PrintError("The TCP client could not wait for upcoming data receiving! ASIO_Error: " + ec.message() + '.');
			return data;
		}

		size_t bytesAvaiable = socket.available();
		if(bytesAvaiable) {
			if(bytesToRead && bytesToRead < bytesAvaiable) {
				data.resize(bytesToRead, ' ');
			} else {
				data.resize(bytesAvaiable, ' ');
			}

			socket.read_some(asio::buffer(data), ec);
			if(ec) {
				Miscellaneous::PrintError("The TCP client could not receive data! ASIO_Error: " + ec.message() + '.');
			} else {
				OnReceive(data);
			}
		}

		return data;
	}

	inline void ReceiveAsync(const uint32_t bytesToRead = 0) override {
		throw std::logic_error("The method 'ClientTCP::ReceiveAsync' has not yet been implemented!");

		/*
		if(!socket.is_open()) {
			Miscellaneous::PrintError("The TCP client was not created successfully!");
			return;
		}

		socket.async_wait(socket.wait_read, [this, bytesToRead] (const asio::error_code& ec) {
			if(ec) {
				Miscellaneous::PrintError("The TCP client could not wait for upcoming data receiving! ASIO_Error: " + ec.message() + '.');
				return;
			}

			size_t bytesAvaiable = socket.available();
			if(bytesAvaiable) {
				std::string data("");
				if(bytesToRead && bytesToRead < bytesAvaiable) {
					data.resize(bytesToRead, ' ');
				} else {
					data.resize(bytesAvaiable, ' ');
				}

				asio::async_read(socket, asio::buffer(data), [this, data] (const asio::error_code& ec, size_t bytesTransferred) {
					if(ec) {
						Miscellaneous::PrintError("The TCP client could not receive data! ASIO_Error: " + ec.message() + '.');
					} else {
						OnReceive(data.substr(0, bytesTransferred));
					}
				});
			}
		});
		*/
	}

	inline asio::ip::tcp::endpoint GetEndpoint() const {
		return endpoint;
	}

	inline asio::ip::tcp::socket& GetSocket() {
		return socket;
	}

	inline asio::io_context& GetIOContext() const {
		return ios;
	}

private:
	inline void OnConnect(const asio::ip::tcp::endpoint& endpoint) override {
		std::cout << std::endl << " The client has connected to the server '" + endpoint.address().to_string() + ':' << endpoint.port() << "'." << std::endl;
	}

	inline void OnSend(const std::string& data) override {
		std::cout << std::endl << " The client has sent data in the size of " << data.size() << " bytes to the server '" + endpoint.address().to_string() + ':' << endpoint.port() << "'." << std::endl;
	}

	inline void OnReceive(const std::string& data) override {
		std::cout << std::endl << " The client has received data in the size of " << data.size() << " bytes from the server '" + endpoint.address().to_string() + ':' << endpoint.port() << "'." << std::endl;
	}

	inline void OnDisconnect() override {
		std::cout << std::endl << " The client has disconnected from the server '" + endpoint.address().to_string() + ':' << endpoint.port() << "'." << std::endl;
	}

	asio::io_context& ios;

	asio::ip::tcp::endpoint endpoint;
	asio::ip::tcp::socket socket;
};

/*
	TO DO:
		- sendasync/receiveasync method crashes on async_read/async_write_some
		- check if the methods are called in a logic way
*/