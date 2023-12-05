// WinWatch.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define BUFFER_SIZE 1024

#include <iostream>
#include <stdlib.h>
#include <string.h>

#include <Windows.h>
#include <windef.h>

#include "./arg.h";

// muy importante bcs i am lazy

// strlcpy.cpp
#undef strlcpy
size_t strlcpy(char*, const char*, size_t);
size_t estrlcpy(char*, const char*, size_t);

// strlcat.cpp
#undef strlcat
size_t strlcat(char*, const char*, size_t);
size_t estrlcat(char*, const char*, size_t);

void usleep(unsigned long long usec) {
	HANDLE timer;
	LARGE_INTEGER ft;

	ft.QuadPart = -static_cast<long long>(usec) * 10; // Convert to 100 nanoseconds interval, negative value for relative time

	timer = CreateWaitableTimer(NULL, TRUE, NULL);
	SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
	WaitForSingleObject(timer, INFINITE);
	CloseHandle(timer);
}

char* argv0;

static void
usage(void)
{
    printf("usage: %s [-t] [-n interval] command\n", argv0);
}

int
main(int argc, char *argv[]){
    char cmd[BUFFER_SIZE];
    char* end;
    unsigned long long interval = 2 * 1E6;
    float period;
    int i;

	ARGBEGIN{
	case 't':
		break;
	case 'n':
		period = strtof(EARGF(usage()), &end);
		if (*end != '\0' || errno != 0)
			printf("invalid interval\n");
		if (period < 0)
			period = 0.1f;
		interval = period * 1E6;
		break;
	default:
		usage();
	} ARGEND;

	if (argc < 1)
		usage();

	if (strlcpy(cmd, argv[0], sizeof(cmd)) >= sizeof(cmd))
		printf("command too long\n");
	for (i = 1; i < argc; i++) {
		if (strlcat(cmd, " ", sizeof(cmd)) >= sizeof(cmd))
			printf("command too long\n");
		if (strlcat(cmd, argv[i], sizeof(cmd)) >= sizeof(cmd))
			printf("command too long\n");
	}

	for (;;) {
		printf("\x1b[2J\x1b[h");
		fflush(NULL);
		system(cmd);
		usleep(interval);
	}
	return 0;
}