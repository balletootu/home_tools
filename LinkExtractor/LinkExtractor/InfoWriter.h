#pragma once

struct s_pair
{
	s_pair(const string& n, const string& r):name(n), rename(r){}
	s_pair(){}
	string name, rename;
};

class CInfoWriter
{
	string		m_name_link;
	string		m_name_rename;

	ofstream	m_file_links;
	ofstream	m_file_renamer;

	vector<string> m_vLink;
	vector<s_pair> m_vRename;

	const static int buf_size = 100;

public:
	CInfoWriter(void);
	~CInfoWriter(void);

	void set_name_file_link(string& name);
	void set_name_file_rename(string& name);

	void link(const string& link);
	void rename(const string& name, const string& raname);

private:
	void flush_link();
	void flush_rename();
};
