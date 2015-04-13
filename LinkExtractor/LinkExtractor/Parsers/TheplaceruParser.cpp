#include "StdAfx.h"
#include "TheplaceruParser.h"

namespace HTMLParsers
{
const string find_pic			=	"_s.";
const string find_next_pages	=	"страницы:";
const string add_head_lnk		=	"http://www.theplace.ru";
const string start_page_lnk		=	"<a href=";

CTheplaceruParser::CTheplaceruParser(void)
{
}

CTheplaceruParser::~CTheplaceruParser(void)
{
}

void CTheplaceruParser::ParceFile( string sIshodName, down_tasks& d_tasks )
{
	ifstream from(sIshodName.c_str());
	if (!from )
	{
		cout << "Don't open: " << sIshodName.c_str();
		return;
	} 

	string sParse;
	string::size_type pos;
	while(getline(from, sParse))
	{
		if ((pos = sParse.find(find_pic)) != string::npos)		//picture
		{
			string::size_type posEnd(0), posStart(0);
			posStart = sParse.rfind("'", pos) + 1;
			posEnd   = sParse.find("'", pos);
			if ((posStart != string::npos) && (posEnd != string::npos))
			{
				down_task dt;
				string sLink;
				sLink = add_head_lnk + sParse.substr(posStart, pos - posStart) + sParse.substr(pos + 2, posEnd - (pos +2));
				dt.filetype = down_task::ftBinary;
				create_down_task(sLink, dt);
				if (sLink.find("/news/") == string::npos)
					d_tasks.push_back(dt);
			}			
		}
		else if(sParse.find(find_next_pages) != string::npos)												//next pages
		{
			string::size_type posEnd(0), posStart(0);
			int index(2);
			string name_file("");
			char buffer[20];
			while((posStart = sParse.find(start_page_lnk, posEnd)) != string::npos)
			{
				posEnd = sParse.find(" ", posStart + 8);
				if (posEnd != string::npos)
				{
					down_task dt;
					posStart += start_page_lnk.size();
					dt.filetype = down_task::ftHTML;
					create_down_task(sParse.substr(posStart, posEnd - posStart), dt);
					if (dt.link.find(add_head_lnk) == string::npos)
						dt.link = add_head_lnk + dt.link;
					_itoa_s(index++, buffer, 10);
					name_file = buffer;			name_file = name_file + ".html";
					dt.filename = name_file;
					d_tasks.push_back(dt);
				}
			}
		}
	}
}

down_task CTheplaceruParser::GetDefaultDownTask()
{
	down_task dt;
	dt.link = "http://www.theplace.ru/photos/Megan_Fox-mid2275.html";	//ссылка для скачки
	dt.filename = "1.html";												//имя для сохранения
	dt.filetype = down_task::ftHTML;

	return dt;
}
}//namespace HTMLParsers
