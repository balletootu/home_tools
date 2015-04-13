#pragma once 

namespace HTMLParsers
{
class IHtmlParser;
class CParserCreator
{
public:
	typedef IHtmlParser* (*CreateParserCallback) ();

private:
	typedef map<string, CreateParserCallback> CreatorsMap;

public:

	static CParserCreator& Instance()
	{
		static CParserCreator parser;
		return parser;
	}

	bool Register(string TypeObj, CreateParserCallback creator)
	{
		return m_Creators.insert(CreatorsMap::value_type(TypeObj, creator)).second != 0;
	}

	bool Unregister(string TypeObj)
	{
		return m_Creators.erase(TypeObj) == 1;
	}

	IHtmlParser* CreateParser(string TypeObj)
	{
		CreatorsMap::const_iterator i = m_Creators.find(TypeObj);
		if (i != m_Creators.end())
			return (i->second)();

		return NULL;
	}

private:
	CParserCreator(void){}
	CParserCreator(const CParserCreator&){}
	CParserCreator& operator = (const CParserCreator&){}
	~CParserCreator(void){}

private:
	CreatorsMap m_Creators;
};
}//namespace HTMLParsers
