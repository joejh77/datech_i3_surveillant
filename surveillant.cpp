#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>
//#include "bb_micom.h"

static int g_app = 1;


int lockfile(int fd)
{
       struct flock fl;


       fl.l_type = F_WRLCK;
       fl.l_start = 0;
       fl.l_whence = SEEK_SET;
       fl.l_len = 0;
       return(fcntl(fd, F_SETLK, &fl));
}


int check_single_running(char *pos, const char *progm)
{
       int fd;
       char PROCESS_ID[16];
       char LOCK_FILE_PATH[128];
       char PROCESS_NAME[32];


       memset(PROCESS_ID, 0x00, sizeof(PROCESS_ID));
       memset(PROCESS_NAME, 0x00, sizeof(PROCESS_NAME));
       memset(LOCK_FILE_PATH, 0x00, sizeof(LOCK_FILE_PATH));


       sprintf(LOCK_FILE_PATH, "%s/%s.pid", "/tmp/mnt", strstr(pos, progm));
		printf( "Debug LOCK_FILE_PATH:[%s]\n", LOCK_FILE_PATH);

		fd = open(LOCK_FILE_PATH, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		if (fd < 0) {
			printf( "Can't Open:[%s]/[%s]\n", LOCK_FILE_PATH, strerror(errno));
			exit(1);
		}


       if(lockfile(fd) < 0){
               if(errno == EACCES || errno == EAGAIN){
                       close(fd);
                       printf( "Already Running:[%s]/[%s]\n", LOCK_FILE_PATH, strerror(errno));
                       return (1);
               }
               printf("Can't Lock:[%s]/[%s]\n", LOCK_FILE_PATH, strerror(errno));
               exit(1);
       }


       ftruncate(fd, 0);
       sprintf(PROCESS_ID, "%u", getpid());
       write(fd, PROCESS_ID, strlen(PROCESS_ID)+1);
       return (0);
}

int is_appexe_stop(const char *cmd, int flg, int *num)
{
	FILE *fp_ps;
	char tmp;
	char num_str[10];
	int bpnum;

	fp_ps = popen(cmd, "r");

	int i = 0;
	do {
		tmp = fgetc(fp_ps);
		if (feof(fp_ps) || tmp == ' ') {
			break;
		}
		num_str[i++] = tmp;
		
		if(i >= sizeof(num_str) - 1)
			break;
	} while(1);
	pclose(fp_ps);
	num_str[i] = '\0';

	if (strlen(num_str)) {
		bpnum = strtoul(num_str, NULL, 10);
	}

	*num = bpnum;

	if (bpnum > flg)
		return 0;

	return 1;
}

void _sig_handler(int sig)
{
	if (sig == SIGTERM) {
		g_app = 0;
	}
}

int main(int argc, char **argv)
{
	int appnum;
	int ret = 0;
	int delay_time;

	if(argc < 2){
		printf( "USAGE : %s <checking time sec>", argv[0]);
		exit(0);
	}

	delay_time = atoi(argv[1]);
	if(delay_time == 0)
		delay_time = 1;
	
	ret = check_single_running(argv[0], "surveillant");
	if(ret !=0){
		printf( "duplicate Running!!");
		exit(0);
	}else{
		printf( "Single Running!!");
	}

	printf("%s START\n", argv[0]);

	signal(SIGTERM, _sig_handler);
	//BB_MICOM_START();
	
	while (g_app) {
		sleep(delay_time);
		
		is_appexe_stop("ps -e | grep -c \"[r]ecorder\"", 0, &appnum);

		//printf("appnum=%d\n", appnum);

		if (appnum == 0) {
			setuid (0);
			system("reboot");
		}
		else {
			// if app has been running
		}
	}

	//BB_MICOM_STOP();
	printf("%s END\n\n", argv[0]);

}
