#pragma once

#include "const.h"

#include <queue>
#include <set>
#include <boost/filesystem/path.hpp>
#include <boost/thread/mutex.hpp>

struct down_task
{
	std::string					server;
	std::string					path;
	boost::filesystem::path		out_file_name;
};

class download_queue
{
public:
	download_queue(){}
	~download_queue(){}

	bool is_task() const {return (tasks_.size() > 0) ? true : false;}

	void add_task(const std::string& task);
	void add_task(const down_task& task);

	bool get_task(down_task& task);

private:
	void delete_inadmiss_char( std::string& str );
	std::string get_name(const std::string& path, const std::string& server);

public:
	typedef std::queue<down_task> t_tasks;
	typedef std::set<std::string> t_set_file_name;

private:
	t_tasks					tasks_;
	boost::mutex			mutex_;
	static t_set_file_name	file_names_;
};
