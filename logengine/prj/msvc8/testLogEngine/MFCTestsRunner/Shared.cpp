
#include "Shared.h"

std::string millisecToStr(long long ms)
{
	int milliseconds = ms % 1000;
	int seconds = (ms / 1000) % 60;
	int minutes = (ms / 60000) % 60;
	int hours = (ms / 3600000) % 24;

	char buf[100];
	if (hours > 0)
		sprintf_s(buf, 100, "%u hours %u minutes %u seconds %u ms", hours, minutes, seconds, milliseconds);
	else if (minutes > 0)
		sprintf_s(buf, 100, "%u minutes %u seconds %u ms", minutes, seconds, milliseconds);
	else
		sprintf_s(buf, 100, "%u seconds %u ms", seconds, milliseconds);

	return buf;
}
