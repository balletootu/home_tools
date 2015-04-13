// LinkExtractor.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "DownloadManager.h"

bool parse_command_line(int argc, char* argv[]);
map<string,string>			g_command_line;

int main(int argc, char* argv[])
{
	try
	{
		if (!parse_command_line(argc, argv)) return 0;

		CDownloadManager mng;
		mng.set_site(g_command_line["site"]);
		mng.set_link(g_command_line["l"]);
		mng.set_anonimize_html(g_command_line["ah"]);
		mng.set_anonimize_binary(g_command_line["ab"]);
//		mng.set_rename_template();
		//TODO: надо сделать закрузку шаблона из файла настроек
		if (g_command_line.find("d") != g_command_line.end())
			mng.set_download_content(true);
		else
			mng.set_download_content(false);
		mng.set_proxy_address(g_command_line["pa"]);
		mng.set_proxy_port(g_command_line["pp"]);

		mng.download();
	}
	catch (std::exception& e)
	{
		std::cout << "Exeption: " << e.what() << endl;
	}

	return 0;
}

void show_command_param()
{
	cout << "LinkExtractor site [param]" << endl;
	cout << "	site - {met-art.com, theplace.ru}" << endl;
	cout << "[param]:" << endl;
	cout << "	-d - download content" << endl;
	cout << "	-ad - default anonimizer for HTML files" << endl;
	cout << "	-ah - anonimizer for HTML files Example: -ahhttp://anonymouse.org/cgi-bin/anon-www.cgi/" << endl;
	cout << "	-ab - anonimizer for Binary files" << endl;
	cout << "	-t - rename to using the template" << endl;
	cout << "	-l - link" << endl;
	cout << "	-pa - proxy server address" << endl;
	cout << "	-pp - proxy server port" << endl;
}

bool parse_command_line(int argc, char* argv[])
{
	if (argc < 2)
	{
		show_command_param();
		return false;
	}
	g_command_line["site"] = argv[1];

	string op, opValue;
	bool isMaping(true);
	for(int i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-')
		{
			isMaping = true;
			switch(argv[i][1])
			{
			case 'a': case 'p':							//двухбуквенные параметры
				opValue = argv[i];
				if(argv[i][2] == 'd')
					opValue = "-ahhttp://anonymouse.org/cgi-bin/anon-www.cgi/";
				op = opValue.substr(1, 2);

				g_command_line[op] = opValue.substr(3, opValue.size() - 3);
				isMaping = false;
				break;
			}

			if (isMaping)
			{
				op = argv[i][1];			opValue = argv[i];
				g_command_line[op] = opValue.substr(2, opValue.size() - 2);
			}
		}
	}

	return true;
}