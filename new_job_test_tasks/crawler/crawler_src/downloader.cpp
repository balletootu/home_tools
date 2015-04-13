#include "downloader.h"
#include "const.h"

namespace http
{

downloader::downloader( const std::string& server, const std::string& path, std::ofstream* out_file )
: server_(server)
, path_(path)
, out_file_(out_file)
{
	boost::asio::io_service io_service;
	tcp::socket socket(io_service);

	init_connection(io_service, socket);
	send_request(socket);
	receive_responce(socket);
}

void downloader::init_connection(boost::asio::io_service& io_service, tcp::socket& socket)
{
	// get a list of endpoints corresponding to the server name
	tcp::resolver resolver(io_service);
	tcp::resolver::query query(server_, s_http);
	tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
	tcp::resolver::iterator end;

	// try each endpoint until we successfully establish a connection
	boost::system::error_code error = boost::asio::error::host_not_found;
	while (error && endpoint_iterator != end)
	{
		socket.close();
		socket.connect(*endpoint_iterator++, error);
	}
	if (error)
		throw boost::system::system_error(error);
}

void downloader::send_request(tcp::socket& socket)
{
	// GET request of a type: 
	// "GET /path  HTTP/1.0\r\nHost: www.server\r\nAccept: */*\r\nConnection: close\r\n\r\n"
	boost::asio::streambuf request;
	std::ostream request_stream(&request);
	request_stream << s_http_get << path_ << s_http_version_1_0 << s_http_new_line;
	request_stream << s_http_host << server_ << s_http_new_line;
	request_stream << s_http_accept << "*/*" << s_http_new_line;
	request_stream << s_http_connection << "close" << s_http_new_line << s_http_new_line;

	// send request
	boost::asio::write(socket, request);
}

void downloader::receive_responce(tcp::socket& socket)
{
	// read the response status line
	boost::asio::streambuf response;
	boost::asio::read_until(socket, response, s_http_new_line);

	std::istream response_stream(&response);
	if (!verify_status_code(response_stream)) return;

	// read headers
	boost::asio::read_until(socket, response, s_http_new_line + s_http_new_line);

	std::string header;
	// skip headers...
	while (std::getline(response_stream, header) && header != "\r")
		;

	if (response.size() > 0)
		*out_file_ << &response;			//.. and write content to file

	// reading remaining data until EOF. write content to file
	boost::system::error_code error;
	while (boost::asio::read(socket, response, boost::asio::transfer_at_least(1), error))
		*out_file_ << &response;							
	if (error != boost::asio::error::eof)
		throw boost::system::system_error(error);
}

bool downloader::verify_status_code(std::istream& r_stream)
{
	std::string http_version;
	r_stream >> http_version;

	unsigned int status_code;
	r_stream >> status_code;

	std::string status_message;
	std::getline(r_stream, status_message);
	if (!r_stream || http_version.substr(0, 5) != "HTTP/")
	{
		std::cout << "Invalid response" << std::endl;
		return false;
	}

	if (status_code != n_http_ok)
	{
		std::cout << "Response returned with status code " << status_code << std::endl;
		return false;
	}

	return true;
}

}	//namespace http