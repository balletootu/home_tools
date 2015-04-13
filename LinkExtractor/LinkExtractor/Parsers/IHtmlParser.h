#include "../Structures.h"
#pragma once 

namespace HTMLParsers
{
	class IHtmlParser
	{
	public:
		virtual ~IHtmlParser(void){}

		virtual void ParceFile(string sIshodName, down_tasks& d_tasks) = 0;
		virtual down_task GetDefaultDownTask() = 0;

		virtual string GetSiteName()=0;
	};

	void create_down_task(const string& link, down_task& dt);
}