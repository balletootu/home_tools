#pragma once
#include "ParserCreator.h"
#include "IHtmlParser.h"

namespace HTMLParsers
{

class CTheplaceruParser : public IHtmlParser
{
public:
	CTheplaceruParser(void);
	virtual ~CTheplaceruParser(void);

	virtual void ParceFile(string sIshodName, down_tasks& d_tasks);

	virtual down_task GetDefaultDownTask();
	virtual string GetSiteName(){return "theplace.ru";}
};

IHtmlParser* TPParseCreator()
{
	IHtmlParser* pParser = new CTheplaceruParser();
	return pParser;
}

const bool regTPParser = CParserCreator::Instance().Register("theplace.ru", TPParseCreator);

}
