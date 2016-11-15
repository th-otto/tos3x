#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char **argv)
{
	unsigned int time, date;
	unsigned int hour, min, sec;
	unsigned int day, month, year;
	int i;
	
	for (i = 1; i < argc; i++)
	{
		time = strtol(argv[i], NULL, 0);
		hour = ((time & 0xf800) >> 11) & 0x001f;
		min = ((time & 0x07e0) >> 5) & 0x003f;
		sec = (time & 0x1f) << 1;
		printf("%02d:%02d:%02d\n", hour, min, sec);
		
		date = time;
		year = 1980 + ((date >> 9) & 0x007f);
		month = (date & 0x01e0) >> 5;
		day = date & 0x001f;
		printf("%02d.%02d.%04d\n", day, month, year);
	}
		
	return EXIT_SUCCESS;
}
