#include "pch.h"
#include ".\Damage Meter/Damage Meter.h"
#include ".\UI\UiWindow.h"
#include ".\Damage Meter\MySQLite.h"
#include ".\UI\PlayerTable.h"
#include ".\Third Party/discord/DiscordPresence.h"
#include ".\Damage Meter\SaveData.h"
#include ".\Soulworker Packet\SWPacketMaker.h"
#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console" )
#endif
void getconfig(CSimpleIniA& ini, int sec)
{
	if (sec == 1) //meter
	{
		bool shouldLog = ini.GetBoolValue("Meter", "LogFile", false);
		if (shouldLog)
		{
			LogInstance.Enable();
		}
		const char* language = ini.GetValue("Meter", "Language");
		if (language) {
			LANGMANAGER.SetCurrentLang((CHAR*)language);
		}
		bool shouldLogMonsterStats = ini.GetBoolValue("Meter", "LogMonsterStats", false);
		bool presence = ini.GetBoolValue("Meter", "RichPresence", true);
		DISCORD.shouldLoad = presence;
		DISCORD.shouldUpdate = presence;
		bool hidename = ini.GetBoolValue("Meter", "HideName", false);
		DISCORD.hideName = hidename;
		bool hideclass = ini.GetBoolValue("Meter", "HideClass", false);
		DISCORD.hideClass = hideclass;
		int wideness = ini.GetLongValue("Meter", "TimerAcc", 1);
		if (wideness < 0 || wideness > 3)
			wideness = 1;
		DAMAGEMETER.mswideness = wideness;
		const char* font = ini.GetValue("Meter", "DefaultFont");
		if (font)
		{
			DAMAGEMETER.selectedFont.filename = font;
			DAMAGEMETER.selectedFont.path = font;
		}
		DAMAGEMETER.shouldLogMstrStats = shouldLogMonsterStats;
		return;
	}
}
bool configloaded = false;
bool createconfig()
{
	DAMAGEMETER.ini.SetBoolValue("Loader", "XignCheck", true);
	DAMAGEMETER.ini.SetBoolValue("Loader", "OpenMeterOnInjection", true);
	DAMAGEMETER.ini.SetLongValue("Meter", "Language", 0);
	DAMAGEMETER.ini.SetBoolValue("Meter", "LogFile", false);
	DAMAGEMETER.ini.SetBoolValue("Meter", "LogMonsterStats", false);
	DAMAGEMETER.ini.SetBoolValue("Meter", "RichPresence", true);
	DAMAGEMETER.ini.SetBoolValue("Meter", "HideName", false);
	DAMAGEMETER.ini.SetBoolValue("Meter", "HideClass", false);
	DAMAGEMETER.ini.SetLongValue("Meter", "TimerAcc", 1);
	DAMAGEMETER.ini.SetValue("Meter", "DefaultFont", "");
	SI_Error rc = DAMAGEMETER.ini.SaveFile("meterconfig.ini");
	if (rc < 0) {
		MessageBoxA(NULL, "Something is wrong with your system, cant make config file.", "ERROR", MB_OK | MB_ICONERROR);
		return false;
	}
	return true;
}
void loadconfig()
{
	DAMAGEMETER.ini.SetUnicode();
	SI_Error rc = DAMAGEMETER.ini.LoadFile("meterconfig.ini");
	if (rc < 0)
	{
		bool test = createconfig();
		if (test == false) return;
	}
	SI_Error rc2 = DAMAGEMETER.ini.LoadFile("meterconfig.ini");
	if (rc2 < 0)
	{
		MessageBoxA(NULL, "Loading config failed for some reason?", "ERROR", MB_OK | MB_ICONERROR);
		return;
	}
	configloaded = true;
	CSimpleIniA::TNamesDepend sections;
	CSimpleIniA::TNamesDepend::const_iterator it;
	DAMAGEMETER.ini.GetAllSections(sections);
	for (it = sections.begin(); it != sections.end(); ++it)
	{
		if (!std::strcmp(it->pItem, "Meter"))
		{
			getconfig(DAMAGEMETER.ini, 1);
		}
	}
	return;
}
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) 
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ PSTR szCmdLine, _In_ int iCmdShow) {

	MiniDump::Begin();
	{
		DWORD errorCode = ERROR_SUCCESS;
		CHAR errorMsg[512] = { 0 };

		do
		{
			const UINT codePage = GetACP();
			switch (codePage) {
			case 936: // ZH-CN
			case 950: // ZH-TW
				_wsetlocale(LC_ALL, L"zh-TW.UTF8");
				errorCode = LANGMANAGER.SetCurrentLang("zh_tw.json");
				break;
			default:
				_wsetlocale(LC_ALL, L"en-US.UTF8");
				errorCode = LANGMANAGER.SetCurrentLang("en.json");
				break;
			}
			loadconfig();
			if (errorCode) {
				sprintf_s(errorMsg, "Init Lang failed. err: %lu", errorCode);
				break;
			}


			if (!SWDB.Init()) {
				sprintf_s(errorMsg, "Init database failed.");
				break;
			}

			if (UIWINDOW.Init(1, 1, 1, 1)) {
				if ((errorCode = SWPACKETMAKER.Init())) {
					sprintf_s(errorMsg, "Init PacketCapture failed, err: %lu", errorCode);
					break;
				}
				if (UIOPTION.isUseSaveData())
				{
					if ((errorCode = SAVEDATA.Init())) {
						sprintf_s(errorMsg, "Init SaveData failed, err: %lu", errorCode);
						if (errorCode == ERROR_FILE_CORRUPT) {
							ANSItoUTF8(LANGMANAGER.GetText("STR_SAVEDATA_VERSION_ERROR"), errorMsg, sizeof(errorMsg));
						}
						else if (errorCode == ERROR_FILE_SYSTEM_LIMITATION) {
							ANSItoUTF8(LANGMANAGER.GetText("STR_SAVEDATA_MULTIPLE_ERROR"), errorMsg, sizeof(errorMsg));
						}
						break;
					}
				}
				UIWINDOW.Run();
			}
			else {
				sprintf_s(errorMsg, "Init UI failed.");
				break;
			}

		} while (false);

		if (errorCode != ERROR_SUCCESS) {
			MessageBoxA(NULL, errorMsg, "SoulMeter", MB_ICONERROR | MB_OK | MB_TOPMOST);
			LogInstance.WriteLog(errorMsg);
		}

		MiniDump::End();
	}

	ShowWindow(UIWINDOW.GetHWND(), 0);
}