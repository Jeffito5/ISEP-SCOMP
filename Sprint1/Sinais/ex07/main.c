#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <ctype.h>

void upcase(char *str) {
    while (*str) {
        *str = toupper(*str);
        str++;        
    }
}

void handle_signal(int signo, siginfo_t *sinfo, void *context) {
	sigset_t a_set;
	int signal;
	sigprocmask(SIG_BLOCK, NULL, &a_set);
    for (signal = 1; signal < NSIG; signal++) {
		char *str = strdup(sys_siglist[signal]);
		upcase(str);
		write(STDOUT_FILENO, "Signal ", 7);
		write(STDOUT_FILENO, str, sizeof(str));
        if (sigismember(&a_set,signal)==1) {
			write(STDOUT_FILENO, " is blocked.\n", 13);
		} else {
			write(STDOUT_FILENO, " is not blocked.\n", 17);
		}
		free(str);
	}
}

extern const char * const sys_siglist[];

int main(int argc, char *argv[]) {
	struct sigaction act;
	memset(&act, 0, sizeof(struct sigaction)); // limpar a estrutura
	sigfillset(&act.sa_mask);
	act.sa_sigaction = handle_signal;
	sigaction(SIGUSR1, &act, NULL);
	
	printf("PID=%d\n",getpid());
	
	pause();
	
	return 0;
}
