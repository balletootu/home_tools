#include "Parsers\IHtmlParser.h"
#include "InfoWriter.h"
#pragma once

class CDownloadManager
{
	HTMLParsers::IHtmlParser*	m_parser;

	//��������� ��������
	string			m_Site;						//��� �����. ������ ������.
	string			m_Link;						//������ � ������� ���������� ��������
	string			m_AnonimizerHtml;			//����������� ��� ������ HTML ������
	string			m_AnonimizerBinary;
	string			m_RenameTemplate;
	string			m_PathHtml;
//	string			m_PathBinary;
//	string			m_PathParsing;				//���� � ������ ������� ���� ����������
	string			m_ProxyAddress;
	string			m_ProxyPort;
	bool			m_bDownloadContent;			//��������� ������� ��� ������ �������� ������ �� ����

	down_tasks		m_down_tasks;
	parce_tasks		m_parse_tasks;

	CInfoWriter		m_OutFiles;

	std::set<std::string> m_files_set;			//��������� ��� ����������� ������

	bool			m_bIsInit;

public:
	CDownloadManager(void);
	~CDownloadManager(void);

	void download();

	void set_site(const string& site){m_Site = site; re_init();}
	void set_link(const string& link){m_Link = link;  re_init();}
	void set_anonimize_html(const string& anonim){m_AnonimizerHtml = anonim;  re_init();}
	void set_anonimize_binary(const string& anonim){m_AnonimizerBinary = anonim;  re_init();}
	void set_rename_template(const string& templ){m_RenameTemplate = templ;  re_init();}
	void set_download_content(bool isDown){m_bDownloadContent = isDown;  re_init();}
	void set_proxy_address(const string& address){m_ProxyAddress = address;}
	void set_proxy_port(const string& port){m_ProxyPort = port;}

private:
	void re_init(){m_bIsInit = false;};
	void init();
	void download_query();
	void create_path_by_tasks();
	void parce_query();
	bool create_file_set( const fs::path & dir_path);
};
