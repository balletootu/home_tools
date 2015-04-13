#include "StdAfx.h"
#include "InfoWriter.h"

CInfoWriter::CInfoWriter(void)
{
	m_vLink.reserve(buf_size);
	m_vRename.reserve(buf_size);
}

CInfoWriter::~CInfoWriter(void)
{
	flush_link();
	if(m_file_links.is_open())
		m_file_links.close();

	flush_rename();
	if (m_file_renamer.is_open())
		m_file_renamer.close();
}

void CInfoWriter::link( const string& link )
{
	m_vLink.push_back(link);
	if ( m_vLink.size() >= buf_size)
		flush_link();
}

void CInfoWriter::rename( const string& name, const string& s_raname )
{
	m_vRename.push_back(s_pair(name, s_raname));
	if ( m_vRename.size() >= buf_size)
		flush_rename();
}

void  CInfoWriter::set_name_file_link(string& name)
{
	if (m_name_link != name)
	{
		if (m_file_links.is_open())
		{
			flush_link();
			m_file_links.close();
		}

		m_name_link = name;
	}
}

void CInfoWriter::set_name_file_rename(string& name)
{
	if (m_name_rename != name)
	{
		if (m_file_renamer.is_open())
		{
			flush_rename();
			m_file_renamer.close();
		}

		m_name_rename = name;
	}
}

void CInfoWriter::flush_link()
{
	if (!m_file_links.is_open() && !m_name_link.empty())
		m_file_links.open(m_name_link.c_str(), std::ios::app);

	if (m_file_links.is_open())
		for (vector<string>::iterator itr = m_vLink.begin(); itr != m_vLink.end(); ++itr)
			m_file_links << (*itr) << endl;

	m_vLink.clear();
}

void CInfoWriter::flush_rename()
{
	if (!m_file_renamer.is_open() && !m_name_rename.empty())
		m_file_renamer.open(m_name_rename.c_str(), std::ios::app);

	if (m_file_renamer.is_open())
		for (vector<s_pair>::iterator itr = m_vRename.begin(); itr != m_vRename.end(); ++itr)
			m_file_renamer << itr->name << " --> " << itr->rename << endl;

	m_vRename.clear();
}