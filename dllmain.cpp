#include "MemoryMgr.h"
#include "inireader.h"

void(*updateptr)(void);

WRAPPER void timecycle_initialise() { EAXJMP(0x5BBAC0); }
WRAPPER void timecycle_update() { EAXJMP(0x561760); }
WRAPPER void add_message(char *text, unsigned int time, unsigned short flag, bool bAddToPreviousBrief) { EAXJMP(0x69F1E0); }

struct timecycle_t {
		int timecycles[9];
		int numcycles;
		char absolutepath[MAX_PATH];
		char datapath[20];
		char *endpath;
	
		void read()
		{
			int num = 0;

			for (int i = 0; i < 9; i++) {
				timecycles[i] = 0;
			}

			for (int i = 1; i <= 9; i++) {
				GetCurrentDirectory(MAX_PATH, absolutepath);
				sprintf(datapath, "\\data\\timecyc%d.dat", i);
				endpath = strncat(absolutepath, datapath, sizeof(datapath));

				if (file_exists(endpath)) {
					timecycles[i-1] = i;
					num++;
				}
			}
			numcycles = num;
		}
};

void update()
{
		timecycle_update();

		struct timecycle_t timecyc;
		char keystroke = 0x78;
		char info = 0;
		static char current[14];
		static unsigned int index;
		static bool keystate = false;

		if (get_path("timecyc.ini") || get_path("scripts\\timecyc.ini")) {
			if ((keystroke = read_var("keystroke")) < 0)
				keystroke = 0x78;
			if ((info = read_var("info")) < 0)
				info = 0;
		}


		if (GetAsyncKeyState(keystroke) & 0x8000) {
			if (!keystate) {
				keystate = true;
				timecyc.read();

				if(timecyc.numcycles > 0) {
					if (index == timecyc.numcycles)
						index = 0;

					snprintf(current, sizeof(current), "timecyc%d.dat", timecyc.timecycles[index]);
					Patch<const char*>(0x5BBAD9 + 1, current);

					if (info)
						add_message(current, 2000, 0, 0);

					timecycle_initialise();
					index++;
				}
			}
		} else keystate = false;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
		switch (ul_reason_for_call) {
		case DLL_PROCESS_ATTACH:
				InterceptCall(&updateptr, update, 0x53C0DA);
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
				break;
		}
		return TRUE;
}
