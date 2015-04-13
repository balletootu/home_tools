#include "StdAfx.h"
#include "MAExtractor.h"
#include "HelpParseMethod.h"

namespace HTMLParsers
{
using namespace boost::gregorian;

const string default_template	=	"%year%/%month%/%year%-%month%-%day% - %model% - %name_set% - %coutn_pic% - %photographer%.jpg";
const string tv_year			=	"%year%";
const string tv_month			=	"%month%";
const string tv_day				=	"%day%";
const string tv_model			=	"%model%";
const string tv_name_set		=	"%name_set%";
const string tv_coutn_pic		=	"%coutn_pic%";
const string tv_photographer	=	"%photographer%";

const string find_pic			=	"tn.jpg";
const string find_start_pic		=	"/met-art_covers";
const string add_head_lnk		=	"http://coverswww.met-art.com";
const string pic_ext			=	".jpg";

CMAExtractor::CMAExtractor(string fileTemplate)
{
	if (fileTemplate.empty())
		m_sFileTemplate = default_template;
	else 
		m_sFileTemplate = fileTemplate;
}

void CMAExtractor::ParceFile(string sIshodName, down_tasks& d_tasks)
{
	ifstream from(sIshodName.c_str());
	if (!from )
	{
		cout << "Don't open: " << sIshodName.c_str();
		return;
	} 
	
	bool isNextPage(false);

	string sParse, sOut, sDate;
	string::size_type pos, posStart;
	while(getline(from, sParse))
	{
		if ((pos = sParse.find(find_pic)) != string::npos)
		{			
			posStart = sParse.rfind(find_start_pic, pos);			
			sOut = add_head_lnk + sParse.substr(posStart, pos - posStart - 1) + pic_ext;
			down_task dt;
			dt.filetype = down_task::ftBinary ;
			create_down_task(sOut, dt);

			posStart = sParse.rfind("/", pos) + 1;
			getline(from, sParse);
			getline(from, sDate);
			getline(from, sParse);
			if (sDate[0]	== '<')	sDate.erase(0, 4);
			if (sDate[sDate.length() - 1] == '>')	sDate.erase(sDate.length() - 4, 4);
			ParseDate(sDate);
			ParceFileInfo(sParse);
			string sFileName = m_sFileTemplate;
			filling_template(sFileName);
			if (sParse.empty()){ continue;}
			dt.filename = "down_binary/" + sFileName;
			d_tasks.push_back(dt);
		} 
		else if (!isNextPage && sParse.find("<MAP NAME=\"cover1.gif\">") != string::npos)
		{
			getline(from, sParse);
			getline(from, sParse);
			pos = sParse.find("HREF=\"") + 6;
			int posEnd(sParse.find('\"', pos));
			if (pos != -1 && posEnd != -1)
			{
				down_task dt;
				dt.filetype = down_task::ftHTML;
				create_down_task(sParse.substr(pos, posEnd - pos), dt);
				d_tasks.push_back(dt);
			}
			isNextPage = true;
		}
	}

	from.close();
}

string CMAExtractor::ParseDate(const string& sIshod)
{
	stringstream sRes;
	string sDay, sMonth, sYear;
	if (sIshod.find("TOMORROW") != string::npos)
	{
		date today = day_clock::local_day();
		day_iterator tomorrow(today);
		++tomorrow;
		char sTmp[20];
		sprintf_s(sTmp, "%d-%.2d-%.2d", (int)tomorrow->year(), (int)tomorrow->month(), (int)tomorrow->day());
		string strTmp(sTmp);
		m_TemplVars[tv_year] = strTmp.substr(0, 4);
		m_TemplVars[tv_month] = strTmp.substr(5, 2);
		m_TemplVars[tv_day] = strTmp.substr(8, 2);
	}
	else
	{
		m_TemplVars[tv_year] = sIshod.substr(6, 4);
		m_TemplVars[tv_month] = sIshod.substr(0, 2);
		m_TemplVars[tv_day] = sIshod.substr(3, 2);
	}

	return sRes.str();
}

void CMAExtractor::ParceFileInfo(string& sIshod)
{
	HelpParseMethod::s_tolower(sIshod);
	string sRes;
	vector<string> vStrings;
	int pos(0), posPrev(0);
	while((pos = sIshod.find("<br>", posPrev)) != string::npos)
	{
		vStrings.push_back(sIshod.substr(posPrev, pos - posPrev));
		posPrev = pos + 4;
	}
	vStrings.push_back(sIshod.substr(posPrev, pos - posPrev));

	string sModel, sSetName, sPhotograph, sCountPic, sTmp;
	if (sIshod[0] == '<' && sIshod[1] == 'f')
	{
		if (vStrings[1].find("new") == string::npos)
		{
			//пример:	<font color="#D6B636">NEW HD 1080 MOVIE<br></font>MILA I IN:<br>"MELLIKIS" BY GONCHAROV<br>740 VIDEO<br>DIVX, QT H264, PSP,<br>IPOD, WINMEDIA, MPG
			sModel = vStrings[1];		
			sModel.erase(0, 7);		//delete "</font>"
			sPhotograph = vStrings[2];
		}
		else
		{
			//new model
			//			<font color="#D6B636">NEW HD 1080 MOVIE<br></font>NEW MODEL<BR>SASHA I IN:<br>"BALLET" BY GONCHAROV<br>741 VIDEO<br>DIVX, QT H264, PSP,<br>IPOD, WINMEDIA, MPG
			sModel = vStrings[2];
			sPhotograph = vStrings[3];
		}
		sModel.erase(sModel.length()- 4, 4);		//delete " IN:"
		sCountPic = "(video)";

		pos = sPhotograph.find(" by ");
		sSetName = sPhotograph.substr(0, pos);
		sSetName.erase(0, 1);	sSetName.erase(sSetName.length() - 1, 1);		//delete "
		pos += 4;
		sPhotograph = sPhotograph.substr(pos, sPhotograph.length() - pos);
	} 
	else
	{
		if (sIshod[0] != '<')
		{
			sModel = vStrings[0];
			sSetName = vStrings[1];		sSetName.erase(0, 1);	sSetName.erase(sSetName.length() - 1, 1);
			sPhotograph = vStrings[2];	sPhotograph.erase(0, 3);
		}
		else
		{
			sSetName = "Presenting";
			if (vStrings[2].find("pixels res.") == string::npos)
			{
				//<br>PRESENTING NEW MODEL<BR>POLLY C BY NUDERO<br><font color="#FFFF80">5600 PIXELS RES.</font><br>117 PHOTOS<br>COMPLETE SERIES
				sPhotograph = vStrings[2];
				pos = sPhotograph.find(" by ");
				sModel = sPhotograph.substr(0, pos);
				pos += 4;
				sPhotograph = sPhotograph.substr(pos, sPhotograph.length() - pos);
			}
			else
			{
				//<br>PRESENTING INDIANA A & CRISTINA A BY ALEX SIRONI<br><font color="#FFFF80">4300 PIXELS RES.</font><br>128 PHOTOS<br>COMPLETE SERIES
				sPhotograph = vStrings[1];
				int startPos = sPhotograph.find("presenting") + 11;
				pos = sPhotograph.find(" by ");
				sModel = sPhotograph.substr(startPos, pos - startPos);
				pos += 4;
				sPhotograph = sPhotograph.substr(pos, sPhotograph.length() - pos);
				vStrings[4] = vStrings[3];
			}
		}

		sCountPic = vStrings[4];	
		pos = sCountPic.find(' ');
		sCountPic.erase(pos, sCountPic.length() - pos);
		sCountPic = "(x" + sCountPic + ")";
	}
	using namespace HelpParseMethod;
	ChangeCase(sModel);
	ChangeCase(sSetName);
	ChangeCase(sPhotograph);

	while ((pos = sSetName.find(':')) != string::npos)		//удаление недопустимых символов из имени файла
		sSetName.replace(pos,1, "");

	m_TemplVars[tv_model] = sModel;
	m_TemplVars[tv_name_set] = sSetName;
	m_TemplVars[tv_coutn_pic] = sCountPic;
	m_TemplVars[tv_photographer] = sPhotograph;
}

void CMAExtractor::filling_template(std::string& templ)
{
	int pos;
	for( template_vars::iterator i = m_TemplVars.begin(); i != m_TemplVars.end(); ++i )
	{
		while ((pos = templ.find(i->first)) != std::string::npos)
			templ.replace(pos, i->first.length(), i->second);
	}
}

down_task CMAExtractor::GetDefaultDownTask()
{
	down_task dt;
	dt.link = "http://anonymouse.org/cgi-bin/anon-www.cgi/http://www.met-art.com/new.html";
	dt.filename = "new.html";
	dt.filetype = down_task::ftHTML;

	return dt;
}
}//namespace HTMLParsers
