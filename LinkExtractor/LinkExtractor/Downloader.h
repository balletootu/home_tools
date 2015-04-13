#include "Structures.h"
#pragma once

using namespace boost;
using namespace boost::asio;
using namespace boost::system;
using boost::asio::ip::tcp;

class CDownloader
{
public:
	CDownloader(io_service& io_service, const down_task& task, std::ofstream* file, string proxy_address = "", string proxy_port = "")
	: resolver_(io_service)
	, socket_(io_service)
	{
		pFile = file;
		// Form the request. We specify the "Connection: close" header so that the
		// server will close the socket after transmitting the response. This will
		// allow us to treat all data up until the EOF as the content.
		
		string down_file, server;
		parce_link(task.link, down_file, server);

		ostream request_stream(&request_);
		if (!proxy_address.empty())
			down_file = task.link;
		request_stream << "GET " << down_file << " HTTP/1.0\r\n";
		request_stream << "Host: " << server << "\r\n";
		request_stream << "Accept: */*\r\n";
		//request_stream << "Accept-Encoding: gzip, deflate\r\n";				//test gzip
		//request_stream << "Cookie: phpbb2mysql_3_sid=ec235d0cc431256034837fe2b181e6a9\r\n";				//test Cookie (nnm-club)
		if (proxy_address.empty())
			request_stream << "Connection: close\r\n\r\n";
		else
			request_stream << "Proxy-Connection: Keep-Alive\r\n\r\n";

		// Start an asynchronous resolve to translate the server and service names
		// into a list of endpoints.
		tcp::resolver::query query(server, "http");
		if (!proxy_address.empty())
			query = tcp::resolver::query(proxy_address, proxy_port);

		resolver_.async_resolve(query, 
			bind(&CDownloader::handle_resolve, this, placeholders::error, placeholders::iterator));
		cout << "down:" << task.link << endl;
	}

private:
  void handle_resolve(const error_code& err, tcp::resolver::iterator endpoint_iterator)
  {
    if (!err)
    {
      // Attempt a connection to the first endpoint in the list. Each endpoint
      // will be tried until we successfully establish a connection.
      tcp::endpoint endpoint = *endpoint_iterator;
      socket_.async_connect(endpoint,
          bind(&CDownloader::handle_connect, this, placeholders::error, ++endpoint_iterator));
    }
    else
    {
      cout << "Error: " << err.message() << "\n";
    }
  }

  void handle_connect(const error_code& err, tcp::resolver::iterator endpoint_iterator)
  {
    if (!err)
    {
      // The connection was successful. Send the request.
      async_write(socket_, request_,
          bind(&CDownloader::handle_write_request, this, placeholders::error));
    }
    else if (endpoint_iterator != tcp::resolver::iterator())
    {
      // The connection failed. Try the next endpoint in the list.
      socket_.close();
      tcp::endpoint endpoint = *endpoint_iterator;
      socket_.async_connect(endpoint,
          bind(&CDownloader::handle_connect, this, placeholders::error, ++endpoint_iterator));
    }
    else
    {
      std::cout << "Error: " << err.message() << "\n";
    }
  }

  void handle_write_request(const error_code& err)
  {
    if (!err)
    {
      // Read the response status line.
      async_read_until(socket_, response_, "\r\n",
          bind(&CDownloader::handle_read_status_line, this, placeholders::error));
    }
    else
    {
      std::cout << "Error: " << err.message() << "\n";
    }
  }

  void handle_read_status_line(const error_code& err)
  {
    if (!err)
    {
      // Check that response is OK.
      istream response_stream(&response_);
      string http_version;
      response_stream >> http_version;
      unsigned int status_code;
      response_stream >> status_code;
      string status_message;
      getline(response_stream, status_message);
      if (!response_stream || http_version.substr(0, 5) != "HTTP/")
      {
        cout << "Invalid response\n";
        return;
      }
      if (status_code != 200)
      {
        cout << "Response returned with status code ";
        cout << status_code << "\n";
        return;
      }

      // Read the response headers, which are terminated by a blank line.
      async_read_until(socket_, response_, "\r\n\r\n",
          bind(&CDownloader::handle_read_headers, this, placeholders::error));
    }
    else
    {
      std::cout << "Error: " << err << "\n";
    }
  }

  void handle_read_headers(const error_code& err)
  {
    if (!err)
    {
      // Process the response headers.
      istream response_stream(&response_);
      string header;
      while (getline(response_stream, header) && header != "\r");

      // Write whatever content we already have to output.
      if (response_.size() > 0)
		  *pFile << &response_;

      // Start reading remaining data until EOF.
      async_read(socket_, response_, transfer_at_least(1),
          bind(&CDownloader::handle_read_content, this, placeholders::error));
    }
    else
    {
      cout << "Error: " << err << "\n";
    }
  }

  void handle_read_content(const error_code& err)
  {
    if (!err)
    {
      // Write all of the data that has been read so far.
		*pFile << &response_;

      // Continue reading remaining data until EOF.
      async_read(socket_, response_, transfer_at_least(1),
          bind(&CDownloader::handle_read_content, this, placeholders::error));
    }
    else if (err != boost::asio::error::eof)
    {
      cout << "Error: " << err << "\n";
    }
  }

  void parce_link(const string& link, string& file, string& server)
  {
	  int pos(0), posNext(0);
	  pos = link.find("http://") + 7;
	  posNext = link.find('/', pos);
	  server = link.substr(pos, posNext - pos);
	  file = link.substr(posNext, link.length());

  }

  std::ofstream* pFile;

  tcp::resolver resolver_;
  tcp::socket socket_;
  boost::asio::streambuf request_;
  boost::asio::streambuf response_;
};
