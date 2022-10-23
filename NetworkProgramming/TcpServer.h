#pragma once
#include "TcpConnection.h"

class TcpServer
{
public:
	TcpServer(boost::asio::io_context& io_context) :
		io_context_(io_context),
		acceptor_(io_context, tcp::endpoint(tcp::v4(), PORT))
	{
		start_accept();
	}

private:
	void start_accept()
	{
		TcpConnection::pointer new_connection = TcpConnection::create(io_context_);
		acceptor_.async_accept(new_connection->socket(),
			[this, new_connection](const boost::system::error_code& error)
			{
				this->handle_accept(new_connection, error);
			});
	}

	void handle_accept(TcpConnection::pointer new_connection,
		const boost::system::error_code& error)
	{
		if (!error)
		{
			new_connection->start();
		}

		start_accept();
	}

private:
	boost::asio::io_context& io_context_;
	tcp::acceptor acceptor_;
};