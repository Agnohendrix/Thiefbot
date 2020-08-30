// TestAC.cpp: definisce il punto di ingresso dell'applicazione console.
//

#include "stdafx.h"
#include <windows.h>
#include <time.h>

int main()
{
	SYSTEM_POWER_STATUS status;


	do {
		GetSystemPowerStatus(&status);
		printf("ACLine: %d\n", status.ACLineStatus);
		printf("Battery: %d\%\n", status.BatteryLifePercent);
		printf("Battery: flag %d\n\n", status.BatteryFlag);
		Sleep(1000);
	} while (true);
	
	return status.ACLineStatus;
}

