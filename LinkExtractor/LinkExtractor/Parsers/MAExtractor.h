#include "ParserCreator.h"
#include "IHtmlParser.h"
#pragma once;

namespace HTMLParsers
{

class CMAExtractor : public IHtmlParser
{
	typedef std::map<std::string, std::string> template_vars;

protected:
	string m_sFileTemplate;
	template_vars m_TemplVars;

public:
	virtual ~CMAExtractor(void){}

	virtual  void ParceFile(string sIshodName, down_tasks& d_tasks);

	virtual  down_task GetDefaultDownTask();
	virtual string GetSiteName(){return "met-art.com";}

protected:
	CMAExtractor(string fileTemplate = "");
	friend IHtmlParser* MAParseCreator();

	string ParseDate(const string& sIshod);
	void ParceFileInfo(string& sIshod);
//	static void create_down_task(string link, down_task& dt);
	void filling_template(std::string& templ);
};

IHtmlParser* MAParseCreator()
{
	IHtmlParser* pParser = new CMAExtractor();
	return pParser;
}

const bool regMAParser = CParserCreator::Instance().Register("met-art.com", MAParseCreator);
}