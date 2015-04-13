#include "StdAfx.h"
#include "DownloadManager.h"
#include "Parsers\ParserCreator.h"
#include "Downloader.h"

CDownloadManager::CDownloadManager(void)
: m_parser(NULL)
, m_Site("")
, m_Link("")
, m_AnonimizerHtml("")
, m_AnonimizerBinary("")
, m_RenameTemplate("")
, m_ProxyAddress("")
, m_ProxyPort("")
, m_bDownloadContent(false)					//временно true, пока не работает сохранение распарсеной инфы
, m_bIsInit(false)
{
}

CDownloadManager::~CDownloadManager(void)
{
	SAFE_DELETE(m_parser);
}

void CDownloadManager::download()
{
	if (!m_bIsInit)
		init();

	while (!m_down_tasks.empty())
	{
		download_query();
		parce_query();
	}
}

void CDownloadManager::init()
{
	SAFE_DELETE(m_parser);
	m_parser = HTMLParsers::CParserCreator::Instance().CreateParser(m_Site);
	if (!m_parser) throw 0;

	fs::create_directories(m_parser->GetSiteName());
	fs::path p(m_parser->GetSiteName(), fs::native );
	create_file_set(p);									//scan downloaded files

	m_PathHtml = m_parser->GetSiteName() + "/";
	m_OutFiles.set_name_file_link(m_PathHtml + "down.lnk");
	m_OutFiles.set_name_file_rename(m_PathHtml + "down.ren");

//#ifdef _DEBUG											//надо это отсюда убрать
//	m_parse_tasks.push_back("./test/test.htm");
//#else
	down_task task;
	task = m_parser->GetDefaultDownTask();
	if (!m_Link.empty())
		task.link = m_Link;
	m_down_tasks.push_back(task);
//#endif
}

void CDownloadManager::download_query()
{
	down_task task;
	std::set<std::string>::const_iterator iter_set;
	while (!m_down_tasks.empty())
	{
		task = m_down_tasks.front();
		m_down_tasks.pop_front();

		if (!m_bDownloadContent && task.filetype == down_task::ftBinary)
		{
			m_OutFiles.link(task.link);

			int pos = task.link.rfind('/') + 1;
			m_OutFiles.rename(task.link.substr(pos, task.link.length()), task.filename.string());
			continue;
		}

		iter_set = m_files_set.find(task.filename.filename());
		if (iter_set == m_files_set.end())
		{
			string f_name = m_PathHtml + task.filename.directory_string();
			std::ofstream file(f_name.c_str(), std::ios::binary);
			if (file)
			{
				boost::asio::io_service io_service;
				CDownloader download(io_service, task, &file, m_ProxyAddress, m_ProxyPort);
				io_service.run();
				file.close();
				if (task.filetype == down_task::ftHTML)
					m_parse_tasks.push_back(f_name);

				m_files_set.insert(task.filename.filename());
			}
		}
	}
}

void CDownloadManager::create_path_by_tasks()
{
	std::set<std::string> folders_set;

	for (down_tasks::iterator task = m_down_tasks.begin(); task != m_down_tasks.end(); task++)
		folders_set.insert(task->filename.parent_path().directory_string());

	for (std::set<std::string>::iterator dp = folders_set.begin(); dp != folders_set.end(); dp++)
		fs::create_directories(*dp);
}

void CDownloadManager::parce_query()
{
	while(!m_parse_tasks.empty())
	{
		m_parser->ParceFile(m_parse_tasks.front(), m_down_tasks);
		m_parse_tasks.pop_front();
	}
	create_path_by_tasks();
}

bool CDownloadManager::create_file_set( const fs::path & dir_path)
{
	if ( !exists( dir_path ) ) return false;
	fs::directory_iterator end_itr;										// default construction yields past-the-end
	for ( fs::directory_iterator itr( dir_path ); itr != end_itr; ++itr )
	{
		if ( is_directory(itr->status()) )
			create_file_set( itr->path());
		else
			m_files_set.insert(itr->filename());
	}
	return true;
}
