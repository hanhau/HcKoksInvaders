#pragma once
#include <Windows.h>
#include <cstdio>

class AntiTemperMethods {
public:
	static inline void hideCurrentThread() {
		HANDLE thread = GetCurrentThread();

		typedef NTSTATUS(NTAPI* pNtSetInformationThread)
			(HANDLE, UINT, PVOID, ULONG);
		NTSTATUS status;

		pNtSetInformationThread NtSIT = (pNtSetInformationThread)
			GetProcAddress(GetModuleHandle(TEXT("ntdll.dll")),
						   "NtSetInformationThread");

		if (NtSIT == nullptr)
			return;

		if (thread == NULL)
			status = NtSIT(GetCurrentThread(),0x11, 0, 0);
		else
			status = NtSIT(thread, 0x11, 0, 0);
	}

	static inline size_t getHardwareInfoHash() {
		FILE* fpipe;
		const char cmd[] = "wmic path win32_physicalmedia get SerialNumber";
		fpipe = (FILE*)_popen(cmd, "r");

		std::string res = "";
		char buffer[128];
		while (!feof(fpipe)) {
			if (fgets(buffer, 128, fpipe))
				res += buffer;
		}

		printf_s(res.c_str());

		_pclose(fpipe);

		return 0;
	}

	static inline bool scanForCheatEngine() {

	}
};