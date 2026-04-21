//---------------------------------------------------------------------------------------------------------------------------
// Обработчик сигналов ошибок
//---------------------------------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <wchar.h> 
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <execinfo.h>
#include <signal.h>
#include <time.h>
#include <math.h>
#include <vector>
#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <beaktrace.h>

//---------------------------------------------------------------------------------------------------------------------------
char* nowstr(long intime = time(0))
{
char *res = (char*)ctime(&intime);
res[strlen(res) - 1] = 0;
return res;
}
//---------------------------------------------------------------------------------------------------------------------------
void OnSignal(int signum, siginfo_t *info, ucontext_t *uc)
{
#define CALL_SIZE 100
void *buffer[CALL_SIZE];
int nptrs = backtrace(buffer, CALL_SIZE);
char **strings = backtrace_symbols(buffer, nptrs);

#if defined __i386__
printf("********************** %s  **********************\n",nowstr());
const char* signal_str[] = {	" ","SIGHUP","SIGINT","SIGQUIT", "SIGILL","SIGTRAP","SIGABRT","SIGIOT",
								"SIGBUS","SIGFPE","SIGKILL","SIGUSR1","SIGSEGV","SIGUSR2","SIGPIPE","SIGALRM",
								"SIGTERM","SIGSTKFLT","SIGCHLD","SIGCONT","SIGSTOP","SIGTSTP","SIGTTIN","SIGTTOU",
								"SIGURG","SIGXCPU","SIGXFSZ","SIGVTALRM","SIGPROF","SIGWINCH","SIGIO","SIGPWR","SIGSYS" };
printf("Signal: %s(%d), si_signo: %d, si_code: %d, si_addr: %08X\n",
	signal_str[signum], signum, info->si_signo, info->si_code, info->si_addr);
printf("Registers: EIP=%08X, EAX=%08X ,EBX=%08X, ECX=%08X, EDX=%08X, ESI=%08X, EDI=%08X, EBP=%08X, ESP=%08X\n",
	uc->uc_mcontext.gregs[REG_EIP],uc->uc_mcontext.gregs[REG_EAX],uc->uc_mcontext.gregs[REG_EBX],
	uc->uc_mcontext.gregs[REG_ECX],uc->uc_mcontext.gregs[REG_EDX],uc->uc_mcontext.gregs[REG_ESI],
	uc->uc_mcontext.gregs[REG_EDI],uc->uc_mcontext.gregs[REG_EBP],uc->uc_mcontext.gregs[REG_ESP]);
fflush(stdout);
#else
printf("********************** %s  **********************\n",nowstr());
const char* signal_str[] = {	" ","SIGHUP","SIGINT","SIGQUIT", "SIGILL","SIGTRAP","SIGABRT","SIGIOT",
								"SIGBUS","SIGFPE","SIGKILL","SIGUSR1","SIGSEGV","SIGUSR2","SIGPIPE","SIGALRM",
								"SIGTERM","SIGSTKFLT","SIGCHLD","SIGCONT","SIGSTOP","SIGTSTP","SIGTTIN","SIGTTOU",
								"SIGURG","SIGXCPU","SIGXFSZ","SIGVTALRM","SIGPROF","SIGWINCH","SIGIO","SIGPWR","SIGSYS" };
printf("Signal: %s(%d), si_signo: %d, si_code: %d, si_addr: %08X\n",
	signal_str[signum], signum, info->si_signo, info->si_code, info->si_addr);
printf("Registers: RIP=%08X, RAX=%08X ,RBX=%08X, RCX=%08X, RDX=%08X, RSI=%08X, RDI=%08X, RBP=%08X, RSP=%08X\n",
	uc->uc_mcontext.gregs[REG_RIP],uc->uc_mcontext.gregs[REG_RAX],uc->uc_mcontext.gregs[REG_RBX],
	uc->uc_mcontext.gregs[REG_RCX],uc->uc_mcontext.gregs[REG_RDX],uc->uc_mcontext.gregs[REG_RSI],
	uc->uc_mcontext.gregs[REG_RDI],uc->uc_mcontext.gregs[REG_RBP],uc->uc_mcontext.gregs[REG_RSP]);
fflush(stdout);
#endif


if(strings)
	{
	for(int j = 0; j < nptrs; ++j)
		{
		printf(".%02d) [%08X] => %s\n", j+1, buffer[j], strings[j]);
		}
	fflush(stdout);
	free(strings);
	}

fflush(stdout);
kill(getpid(),9);
}
//---------------------------------------------------------------------------------------------------------------------------
void SetupSignalHandler()
{
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	*(void**)&act.sa_sigaction = (void*)OnSignal;
	act.sa_flags = SA_RESTART | SA_SIGINFO;
	sigaction(SIGSEGV, &act, 0);
	sigaction(SIGQUIT, &act, 0);
	sigaction(SIGILL, &act, 0);
	sigaction(SIGABRT, &act, 0);
	sigaction(SIGFPE, &act, 0);
	sigaction(SIGKILL, &act, 0);
	sigaction(SIGSYS, &act, 0);
	sigaction(SIGALRM, &act, 0);
}
//---------------------------------------------------------------------------------------------------------------------------