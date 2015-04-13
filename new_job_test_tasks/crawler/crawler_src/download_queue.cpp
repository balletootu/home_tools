#include "download_queue.h"
#include <boost/algorithm/string/erase.hpp>

download_queue::t_set_file_name download_queue::file_names_;

void download_queue::add_task( const std::string& url )
{
	//get server name from URL
	std::string::size_type pos_start = url.find(s_double_slash);
	if (pos_start == std::string::npos) 
		pos_start = 0;
	else 
		pos_start += 2;

	std::string::size_type pos = url.find(c_slash, pos_start);					//find first '/'
	down_task d_task;
	d_task.server = url.substr(pos_start, pos - pos_start);

	d_task.path = url.substr(pos, url.size() - pos);							//get path
	d_task.out_file_name = boost::filesystem::path( 
		get_name(d_task.path, d_task.server), boost::filesystem::native);		//get file name

	add_task(d_task);
}

void download_queue::add_task( const down_task& task )
{
	tasks_.push(task);
}

bool download_queue::get_task( down_task& task )
{
	if (tasks_.size() == 0)
		return false;

	boost::mutex::scoped_lock scoped_lock(mutex_);
	task = tasks_.front();
	tasks_.pop();
	return true;
}

void download_queue::delete_inadmiss_char( std::string& str )
{
	if (str.empty()) return;

	const int num_inadmiss_char(7);
	static char symbol[num_inadmiss_char] = {':', '*', '?', '"', '<','>','|'};
	std::string::size_type pos;

	for (int i = 0; i < num_inadmiss_char; i++)
		while ((pos = str.find(symbol[i])) != std::string::npos)
			str.replace(pos, 1, "");
}

std::string download_queue::get_name(const std::string& path, const std::string& server)
{
	std::string s_path(path);
	delete_inadmiss_char(s_path);

	std::string::size_type pos_file_name = s_path.rfind(c_slash) + 1;						//find last '/'
	std::string file_name( s_path.substr( pos_file_name, s_path.size() - pos_file_name) );

	if (file_name.empty())
		file_name = s_def_name;

	std::string gen_path(s_path.substr(0, pos_file_name) + file_name);

	t_set_file_name::iterator itr = file_names_.find(gen_path);
	if (itr != file_names_.end())
	{
		std::string file_ext("");
		std::string::size_type pos;
		if ((pos = file_name.rfind(c_dot)) != std::string::npos)
		{
			file_ext = file_name.substr(pos, file_name.size() - pos);
			file_name = file_name.substr(0, pos);
		}

		std::string add_name(file_name);

		gen_path = s_path.substr(0, pos_file_name) + add_name + file_ext;

		int index(1);
		while ((itr = file_names_.find(gen_path)) != file_names_.end()) 
		{
			std::ostringstream name_stream;
			name_stream << file_name << "[" << index << "]";
			add_name = name_stream.str();

			gen_path = s_path.substr(0, pos_file_name) + add_name + file_ext;
			index++;			
		} 

	}
	file_names_.insert(gen_path);

	return s_output_directory + c_slash + server + gen_path;

}
