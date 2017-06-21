// CS 111 Lab4 part a
// Jaehyung Park
// 504212821
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <signal.h>
#include <mraa/aio.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>

sig_atomic_t volatile run_flag = 1;

double get_temperature(uint16_t value, char unit) {
	const int B = 4275; // B value of the thermistor
	double R = 1023/(double)value - 1;
	double celsius = 1/(log(R)/B + 1/298.15) - 273.15;
	return unit == 'F' ? celsius * 9.0/5.0 + 32 : celsius;
}

// get_interrupt function needed 
void get_interrupt(int sig) {
	if (sig == SIGINT) {
		run_flag = 0;
	}
}

int main(int argc, char* argv[]){
	mraa_aio_context m_aio;
	// get sensor value from A0 pin
	m_aio = mraa_aio_init(0);
	FILE* logfile = fopen("log.txt","w");

	int max_iterations = argc == 2 ? atoi(argv[1]) : INT_MAX;
	for (int i = 0; run_flag && i < max_iterations; i++){
		char logbuf[1024] = {0};
		time_t t = time(NULL);
		struct tm* tm = localtime(&t);
		sprintf(logbuf, "%02d:%02d:%02d", tm->tm_hour, tm->tm_min, tm->tm_sec);
		uint16_t value = mraa_aio_read(m_aio);
		sprintf(logbuf + strlen(logbuf), " %02.1f", get_temperature(value, 'F'));

		printf("%s\n", logbuf);
		fprintf(logfile, "%s\n", logbuf);
		fflush(logfile);
		sleep(1);
	}
	fclose(logfile);
	mraa_aio_close(m_aio);
}

