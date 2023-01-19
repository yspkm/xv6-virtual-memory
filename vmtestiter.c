#include "user.h"
int main(int argc, char**argv)
{
	int pid;
	uint time;
	while(1)
	{
		pid = fork();
		time = uptime();
		if (!pid) {
			exec("vmtest", argv);
		}
		else {
			wait();
			while(uptime()-time < 1000)
				;
		}
	}
	exit();
	return 0;
}
