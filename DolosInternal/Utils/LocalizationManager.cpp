#include "LocalizationManager.h"



bool InitializeLocalization(const char* szLanguage){

	char name[MAX_PATH];

	GetModuleFileName(NULL, name, MAX_PATH);

	//Locate game files containing model and skin info
	std::string szLangPath = name;
	szLangPath = szLangPath.substr(0, szLangPath.find_last_of('\\'));
	szLangPath += LANG_PATH; 
	szLangPath += szLanguage;
	szLangPath += ".txt";

	JsonObjectW* pLang = ParseJsonFileW(szLangPath.c_str());
	if (pLang) {
		JsonObjectW* pStrings = pLang->GetJsonObject(L"Tokens");
		if (pStrings) {
			for (size_t i = 0; i < pStrings->m_vValues.size(); i++) {
				mStrings[const_hash(string_cast(pStrings->m_vValues[i].m_szName).c_str())] = pStrings->m_vValues[i].m_szValue;
			}
			delete pLang;
			return true;
		}
		delete pLang;
	}
	return false;
	
}

bool UpdateLocalization(const char* szLanguage) {
	mStrings.clear();
	return InitializeLocalization(szLanguage);
}

std::wstring GetLocalizedStringW(const wchar_t* wszName) {
	//If string starts with # add 1 to ptr to ignore
	return mStrings[const_hash(string_cast(wszName + (wszName[0] == '#')).c_str())];
}

std::wstring GetLocalizedStringW(const char* szName) {
	return mStrings[const_hash(szName + (szName[0] == '#'))];
}

std::string GetLocalizedString(const wchar_t* wszName){
	return string_cast(mStrings[const_hash(string_cast(wszName + (wszName[0] == '#')).c_str())]);
}

std::string GetLocalizedString(const char* szName) {
	return string_cast(mStrings[const_hash(szName + (szName[0] == '#'))]);
}

std::wstring wstring_cast(std::string szString){
	int iSize = MultiByteToWideChar(CP_ACP, 0, szString.c_str(), szString.length() + 1, 0, 0);

	wchar_t* pBuffer = new wchar_t[iSize];
	MultiByteToWideChar(CP_ACP, 0, szString.c_str(), szString.length() + 1, pBuffer, iSize);
	std::wstring wszReturn(pBuffer);
	delete[] pBuffer;


	return wszReturn;
}

std::string string_cast(std::wstring wszString)
{
	int iSize = WideCharToMultiByte(CP_ACP, 0, wszString.c_str(), wszString.length() + 1, 0, 0, 0, 0);

	char* pBuffer = new char[iSize];
	WideCharToMultiByte(CP_ACP, 0, wszString.c_str(), wszString.length() + 1, pBuffer, iSize, 0, 0);
	std::string szReturn(pBuffer);
	delete[] pBuffer;


	return szReturn;
}
