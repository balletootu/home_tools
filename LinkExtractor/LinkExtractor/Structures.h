#pragma once 

struct down_task{		// �������� ������ link. ����� ������������ ������ � ����. ������� ��������� � ���������

	enum  fileType {ftHTML, ftBinary};
	
	down_task():filetype(ftHTML){}

	string		link;
	fs::path	filename;
	fileType	filetype;
};

typedef std::deque<down_task>	down_tasks;
typedef std::deque<string>		parce_tasks;