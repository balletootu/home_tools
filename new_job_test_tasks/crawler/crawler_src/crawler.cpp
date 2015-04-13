#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/thread/thread.hpp>

#include "const.h"
#include "download_queue.h"
#include "downloader.h"

void parce_tasks();
void execute_task();
download_queue g_download_queue;
boost::mutex io_mutex;

int main(/*int argc, char* argv[]*/)
{
	try
	{
		parce_tasks();

		if (g_download_queue.is_task())
		{
			const int num_threads = 10;
			boost::thread_group gr_threads;

			for (int i = 0; i < num_threads; ++i)
				gr_threads.create_thread(&execute_task);

			gr_threads.join_all();
		}
	}
	catch (std::exception& e)
	{
		std::cout << "Exception: " << e.what() << std::endl;
	}

	return 0;
}

void parce_tasks()
{
	using namespace std;
	ifstream file(s_input_file.c_str());

	if (!file)
	{
		cout << "The file seed.txt isn't opened." << endl;
		return;
	}

	string s_tmp;
	while (!file.eof())
	{
		getline(file, s_tmp, c_new_line);
		if (s_tmp.empty())
			continue;

		g_download_queue.add_task(s_tmp);
	}

	file.close();
}

void execute_task()
{
	down_task task; 
	boost::filesystem::ofstream file;

	while (g_download_queue.get_task(task))
	{
		create_directories(task.out_file_name.branch_path());
		file.open(task.out_file_name, std::ios::binary);

		{
			boost::mutex::scoped_lock lock(io_mutex);
			std::cout << "Start download: " << task.server << task.path << std::endl;
		}
		http::downloader dwnl(task.server, task.path, &file);
		{
			boost::mutex::scoped_lock lock(io_mutex);
			std::cout << "Finish download: " << task.server << task.path << std::endl;
		}

		file.close();
	}
}