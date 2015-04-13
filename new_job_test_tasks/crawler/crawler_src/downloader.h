#pragma once

#include <iostream>
#include <boost/asio.hpp>
#include <boost/filesystem/fstream.hpp>

using boost::asio::ip::tcp;

namespace http
{
	class downloader
	{
	public:
		downloader(const std::string& server, const std::string& path, std::ofstream* out_file);

	private:
		void init_connection(boost::asio::io_service& io_service, tcp::socket& socket);
		void send_request(tcp::socket& socket);
		void receive_responce(tcp::socket& socket);
		bool verify_status_code(std::istream& r_stream);

	private:
		std::string		server_;
		std::string		path_;

		std::ofstream*	out_file_;
	};
}
