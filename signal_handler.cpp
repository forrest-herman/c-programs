#include <iostream>
#include <csignal>
#include <unistd.h>
#include <cstdlib>
using namespace std;


// user-defined signal handler for alarm
void alarm_handler(int signo) {
	if (signo == SIGALRM) {
		printf ("Alarm\n");
	}
}

// user - defined signal handler for ctrc .
void ctrlc_handler(int signo) {
	if (signo == SIGINT) {
		printf (" CTRL+C pressed! \n");
	}
}
// user - defined signal handler for ctrz .
void ctrlz_handler(int signo) {
	if (signo == SIGTSTP) {
		printf(" CTRL+Z pressed! \n");
		exit(1);
	}
}

int main () {
	// register the signal handler
	if (signal(SIGALRM, alarm_handler ) == SIG_ERR) {
		printf("failed to register alarm handler.");
		exit(1);
	}
	if (signal(SIGINT, ctrlc_handler ) == SIG_ERR) {
		printf("failed to register ctrc handler.");
		exit(1);
	}
	if (signal(SIGTSTP, ctrlz_handler ) == SIG_ERR) {
		printf("failed to register ctrlz handler.");
		exit(1);
    }
    while(1){
        sleep(2); //wait until alarm goes off
        alarm(2); //wait until alarm goes off
    }
    return 0;
}
