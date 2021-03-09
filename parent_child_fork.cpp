#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <math.h>

using namespace std;

int main(void){
	int fd[2], fdReturn[2];
	int nbytes, nbytesReturn=0, sum=0;
	pid_t childpid;
	signed char input_val;
	signed char readbuffer;
	char sumbuffer [80];
	char sumStr[80];
	pipe (fd);
	pipe (fdReturn);
	
	if ((childpid = fork ()) == -1) {
		perror (" fork ");
		exit (0);
	}

	write (fdReturn [1], "NO", 3);

	 while(true){
		if(childpid == 0) {
			nbytesReturn = read (fdReturn [0], sumbuffer, sizeof(sumbuffer));

			// check if parent has returned a value for sum
			if (strcmp("NO",sumbuffer)){				
				printf ("Received Sum: %s\n", sumbuffer);
				return 0; //end program
			}
			else{
				printf("Enter a number: ");
				scanf(" %hhd", &input_val);
				//printf("%d\n", input_val);
				
				close (fd [0]);
				write (fd [1], &input_val, (1));
				sleep(1); //necessary to avoid weirdness
			}
		}

		else {
			close (fd [1]);
			nbytes = read (fd [0], &readbuffer, 1);
			printf ("Received char : %d\n", readbuffer);

			// Check if input is '-1'
			if (readbuffer==-1){
				// printf("Sum Parent = %d\n", sum);
				close (fdReturn [0]);
				sprintf(sumStr, "%d", sum);
				// printf("%s", sumStr);
				write (fdReturn [1], sumStr,(strlen(sumStr)+1));
				sleep(1);
				exit(0); // sum has been sent
			}
			else{
				sum += readbuffer;
				close (fdReturn [0]);
				write (fdReturn [1], "NO", 3); // Send NO meaning sum is not ready yet
			}

		}
	
	 }
}