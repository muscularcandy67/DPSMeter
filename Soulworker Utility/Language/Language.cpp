#include "pch.h"
#include "Language.h"

auto Language::GetLangFile(CHAR* langFile, BOOL outputERROR)
{
	// parse path
	CHAR path[MAX_PATH] = { 0 };
	sprintf_s(path, "%s%s", _langFolder, langFile);

	json j;

	do
	{
		// get raw data
		std::string langRaw;
		if (!file_contents(std::filesystem::path(path), &langRaw)) {
			if (outputERROR)
				LogInstance.WriteLog("[Language::SetCurrentLang] Lang file %s not found.", langFile);
			break;
		}

		// parse raw to json
		j = json::parse(langRaw);
		if (j.empty()) {
			if (outputERROR)
				LogInstance.WriteLog("[Language::SetCurrentLang] Lang file %s is empty.", langFile);
			break;
		}

	} while (false);

	return j;
}

std::unordered_map<std::string, std::string> Language::MapLangData(CHAR* langFile, BOOL useReplace)
{
	std::unordered_map<std::string, std::string> list;

	// get json data
	auto langData = GetLangFile(langFile);
	if (!langData.empty()) 
	{
		for (json::iterator itr = langData.begin(); itr != langData.end(); itr++) 
			list[itr.key()] = itr.value().get<std::string>();

		if (useReplace)
		{
			try {
				auto replaceData = GetLangFile("replace.lang", FALSE);
				if (!replaceData.empty())
				{
					for (json::iterator itr = replaceData.begin(); itr != replaceData.end(); itr++)
						list[itr.key()] = itr.value().get<std::string>();
				}
			}
			catch (...) {
				LogInstance.WriteLog("[Language::MapLangData] Load replace.lang failed.");
			}
		}
	}

	return list;
}

DWORD Language::SetCurrentLang(CHAR* langFile)
{
	DWORD error = ERROR_SUCCESS;

	do {
		std::unordered_map<std::string, std::string> newLang;

		// get json data
		try {
			newLang = MapLangData(langFile, TRUE);
			if (newLang.empty()) {
				error = ERROR_NOT_FOUND;
				break;
			}
		}
		catch (...)
		{
			LogInstance.WriteLog("[Language::SetCurrentLang] Load lang error.");
			error = ERROR_ACCESS_DENIED;
			break;
		}

		// set current lang
		strcpy_s(_currentLang, langFile);

		_textList = newLang;

		_notFoundText.clear();

	} while (false);

	return error;
}

CHAR* Language::GetText(CHAR* text, std::unordered_map<std::string, std::string>* vector)
{
	if (vector == nullptr)
		vector = &_textList;
	
	if (vector->find(text) == vector->end()) {
		std::string findStr(text);
		if (std::find(_notFoundText.begin(), _notFoundText.end(), findStr) == _notFoundText.end())
		{
			LogInstance.WriteLog("[Language::GetText] Lang text %s not found.", text);
			_notFoundText.push_back(findStr);
		}
		return text;
	}

	return (CHAR*)vector->at(text).c_str();
}

std::unordered_map<std::string, std::string> Language::GetAllLangFile()
{
	std::unordered_map<std::string, std::string> list;

	for (auto& p : std::filesystem::directory_iterator(_langFolder)) {
		if (p.is_regular_file()) {
			if (p.path().extension().string() == ".json") {

				std::string fileName = p.path().filename().string();

				try {
					auto langData = MapLangData((CHAR*)fileName.c_str());

					if (!langData.empty()) {

						CHAR* langName = GetText("STR_LANG_NAME", &langData);

						list.emplace(fileName, langName);
					}
				}
				catch (...) {
					LogInstance.WriteLog("[Language::SetCurrentLang] Load lang file %s failed.", fileName.c_str());
				}
			}
		}
	}

	return list;
}