#include "StdAfx.h"
#include "IHtmlParser.h"

namespace HTMLParsers
{

void create_down_task(const string& link, down_task& dt)
{
	int pos(0);
	dt.link = link;

	pos = link.rfind('/') + 1;
	dt.filename = link.substr(pos, link.length());
}

}//namespace HTMLParsers