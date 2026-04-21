//---------------------------------------------------------------------------------------------------------------------------
// Headers
//---------------------------------------------------------------------------------------------------------------------------
#include <wchar.h> 
#include <cwchar>
#include <cstdlib>
#include <cstring>
#include <sys/mman.h>
#include <sys/time.h>
#include <execinfo.h>
#include <signal.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <sstream>
#include <iostream>
#include <pthread.h>
#include <uchar.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <beaktrace.h>

#include "lib_struct.h"
static int asm_sev_log = 0;
using namespace GNET;

//---------------------------------------------------------------------------------------------------------------------------
//--FILE CODE--
//---------------------------------------------------------------------------------------------------------------------------
void lib_sev_log(char *Src)
{
	FILE *fp = fopen("./service.log", "a+");
	fwrite(Src, 1, strlen(Src), fp);
	fclose(fp); 
}
//---------------------------------------------------------------------------------------------------------------------------
void lib_sev_dump(void *Src, size_t len, char* name)
{
	char fname[256];
	sprintf(fname,"./dump/%s",name);
	FILE *fp = fopen(fname, "wb");
	fwrite(Src, 1, len, fp);
	fclose(fp);
}
//---------------------------------------------------------------------------------------------------------------------------
//--Other Code
//---------------------------------------------------------------------------------------------------------------------------
int get_data_val(unsigned char* data, int pos, int type)
{
  unsigned int res = 0;
  switch (type)
  {
    case 0: res = *(int8_t*)&data[pos]; break;
    case 1: res = *(int16_t*)&data[pos]; break;
    case 2: res = *(int32_t*)&data[pos]; break;
    case 3: res = *(int64_t*)&data[pos]; break;
  }
  return res;
}
//---------------------------------------------------------------------------------------------------------------------------
void set_data_val(unsigned char* data, int pos, unsigned char type, unsigned int value)
{
  switch (type)
  {
    case 0: *(int8_t*)&data[pos] = value; break;
    case 1: *(int16_t*)&data[pos] = value; break;
    case 2: *(int32_t*)&data[pos] = value; break;
    case 3: *(int64_t*)&data[pos] = value; break;
  }
}
//---------------------------------------------------------------------------------------------------------------------------
//--Защита от ООГ Атак
//---------------------------------------------------------------------------------------------------------------------------
int oog_get_opcode_rate(unsigned int opcode){ return 0; }
bool oog_event_use_packet(int ls) {	return 0; }
int oog_event_timer(int ls) { return 0;	}
void oog_load_state() { }
//---------------------------------------------------------------------------------------------------------------------------
//--Thread 
//---------------------------------------------------------------------------------------------------------------------------
void* lib_TimerLoop(void*)
{
pthread_detach(pthread_self());
while(true)
	{
	usleep(1000000);

	// Действия каждую секунду
	// ...
	}
return 0;
}
//---------------------------------------------------------------------------------------------------------------------------
int game_data_base_deamon = 0;
void event_lib_start()
{
	pthread_t thread;
	pthread_create(&thread,0,lib_TimerLoop,0);
	SetupSignalHandler();
	game_data_base_deamon++;
}



