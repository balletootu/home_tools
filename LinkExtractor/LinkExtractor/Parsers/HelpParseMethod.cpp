#include "StdAfx.h"

namespace HelpParseMethod
{

void ChangeCase(string& sConv)
{
	bool bNextUpper(true);

	int nLengthStr(sConv.length());
	for (int i(0); i < nLengthStr; i++)
	{
		if (isspace(sConv[i]))
		{bNextUpper = true;	continue;}

		if (bNextUpper)
		{sConv[i] = toupper(sConv[i]);	bNextUpper = false;	}
		else
			sConv[i] = tolower(sConv[i]);
	}
}

void s_tolower(string& sConv)
{
	int nLengthStr(sConv.length());
	for (int i(0); i < nLengthStr; i++)
		sConv[i] = tolower(sConv[i]);
}
}	//namespace HelpParseMethod