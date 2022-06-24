#include "Networking/Client.hpp"

ClientTCP::ClientTCP(asio::io_context& ios):
	ios(ios),
	socket(ios) {

}

ClientTCP::ClientTCP(asio::io_context& ios, const std::string& host, const uint16_t port):
	ios(ios),
	socket(ios) {
	Set(host, port);
}

ClientTCP::~ClientTCP() {

}