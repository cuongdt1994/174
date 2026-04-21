#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/resource.h>
#include <iostream>
#include <signal.h>
#include <threadpool.h>
#include "start.h"
#include <gsp_if.h>
#include <glog.h>
#include <amemory.h>
#include "world.h"
#include "worldmanager.h"
#include "arandomgen.h"
#include <malloc.h>
#include <unordered_map>
#include "threadusage.h"
#include "global_controller.h"
#include "arenamanager.h"
#include "luamanager.h"
#include "player_lottery.h"
#include "glyph_manager.h"
#include "player_lib.h"
#include "world_points_manager.h"
#include "activity_event_manager.h"
#include "celestial_memorial_manager.h"
#include "emulate_settings.h"
#include "codex_manager.h"

using namespace GNET;

void SetupSignalHandler();

bool g_mobile_server = false;

const unsigned int  CSIZE = 262140;
static int size_counter[CSIZE] = {0};
static int large_counter = 0;
static int total_mem_size = 0;

static void AllocSize(size_t n)
{
	interlocked_add(&total_mem_size,n);
	if(n <CSIZE)
	{
		interlocked_add(&size_counter[n],1);
	}
	else
	{
		interlocked_add(&large_counter,1);
	}
}

static void DeleteSize(size_t n)
{
	interlocked_add(&total_mem_size,-(size_t)n);
	if(n <CSIZE)
	{
		interlocked_add(&size_counter[n],-1);
	}
	else
	{
		interlocked_add(&large_counter,-1);
	}
}

//static bool no_record = true;
void * operator new[](size_t n)
{
	void * rst = abase::fast_allocator::raw_alloc(n + sizeof(int));
	*(int*) rst = n;
	AllocSize(n);
	return ((char*)rst) + sizeof(int);
}

void * operator new(size_t n)
{
	void * rst = abase::fast_allocator::raw_alloc(n + sizeof(int));
	*(int*) rst = n;
	AllocSize(n);
	return ((char*)rst) + sizeof(int);
}

void operator delete(void * p, size_t n)
{
	if(!p) return;
	void * xp = ((char*)p) -sizeof(int);
	int size = *(int*)xp;
	DeleteSize(size);
	abase::fast_allocator::raw_free(xp);
//	free(p);
//	if(!no_record) fprintf(stderr,"free_ptr   %p  caller %p\n",p,*(&n-1));
}

void operator delete(void * p)
{
	if(!p) return;
	void * xp = ((char*)p) -sizeof(int);
	int size = *(int*)xp;
	DeleteSize(size);
	abase::fast_allocator::raw_free(xp);
//	free(p);
//	if(!no_record) fprintf(stderr,"free_ptr   %p  caller %p\n",p,*(&p-1));
}

void operator delete[](void * p)
{
	if(!p) return;
	void * xp = ((char*)p) -sizeof(int);
	int size = *(int*)xp;
	DeleteSize(size);
	abase::fast_allocator::raw_free(xp);
//	free(p);
//	if(!no_record) fprintf(stderr,"free_ptr   %p  caller %p\n",p,*(&p-1));
}

int GetCPUCount()
{
	return 8;
}
void PRINT_MEM_USAGE(const char * name)
{
	static rusage g_usage;
	if(name)
	{
		rusage tmp = g_usage;
		getrusage(RUSAGE_SELF, &g_usage);
		__PRINTF("%s�����ڴ� %ld KB, ������ռ�� %ld KB\n", name, g_usage.ru_maxrss - tmp.ru_maxrss, g_usage.ru_maxrss);
	}
	else
	{
		getrusage(RUSAGE_SELF, &g_usage);
		__PRINTF("��̬����ռ���ڴ� %ld KB\n", g_usage.ru_maxrss);
	}
}

int InitWorld(const char*, const char *);
int FirstStepInit(const char *,const char * );
//#include <mcheck.h>
namespace ONET{ 
extern int __thread_lock_count;
extern int __thread_task_count;
}
namespace GMSV{
extern unsigned long long s2c_cmd_number_counter1[1024];
extern unsigned long long s2c_cmd_number_counter2[1024];
}
int main(int argn , char ** argv)
{
	static const char * m_service = "gs";
	
	SetupSignalHandler();

	printf("Emulate by Newester Entertainment \n");
	if(system("/bin/touch foo"))
	{
		printf("�ļ�ϵͳ����д���޷����к�̵ĳ�ʼ������\n");
		return -1;
	}

	time_t now = time(NULL);
	struct tm tm1; 
	localtime_r(&now, &tm1);
	if(tm1.tm_gmtoff == 28800) //����ʱ�����޸�TZ �������� 
	{
		putenv("TZ=Asia/Shanghai");
	}
	char * pTZ =  getenv("TZ");
	__PRINTINFO("env TZ = %s\n", (pTZ ? pTZ : "")); 

	const char * conf_file = "gs.conf";
	const char * gmconf_file = "gmserver.conf";
	const char * servername = "gs01";
	const char * alias_conf = "gsalias.conf";
	if(argn >1) servername = argv[1];
	if(argn >2) conf_file = argv[2];
	if(argn >3) gmconf_file = argv[3];
	if(argn >4) alias_conf = argv[4];

	
		LuaManager::GetInstance()->Init();
	

	//�ֻ��û��������̶�ǰ׺Ϊ"ms"
	const char * mobile_prefix = "ms";
	if(strncmp(servername, mobile_prefix, strlen(mobile_prefix)) == 0)
	{
		g_mobile_server = true;
		__PRINTINFO("�ֻ��û�������(%s)����...\n", servername);
	}

	if(int rst = FirstStepInit(conf_file,alias_conf))
	{
		__PRINTINFO("��һ����ʼ��ʧ�ܣ������:%d\n",rst);
		return rst;
	}

	if(argn >5)
	{
		do
		{
			if(fork() == 0)
			{
				for(int i = 5; i < argn; i ++)
				{
					memset(argv[i],0,strlen(argv[i]));
					argv[i] = 0;
				}
				if(fork() == 0) break; else return 0;
			}

			int i;
			for(i = 5; i < argn; i ++)
			{
				if(fork() == 0)
				{
					if(fork() == 0)
					{
						servername = strdup(argv[i]);
						memset(argv[1],0,strlen(argv[1]));
						strcpy(argv[1],argv[i]);
						for(int j =5; j < argn; j ++)
						{
							memset(argv[j],0,strlen(argv[j]));
							argv[j] = 0;
						}
						break;
					}
					else
					{
						return 0;
					}
				}
			}
			if(i == argn) return 0; //�������˳�
		}
		while(0);
	}

	if(g_mobile_server && strncmp(servername, mobile_prefix, strlen(mobile_prefix)) != 0
			|| !g_mobile_server && strncmp(servername, mobile_prefix, strlen(mobile_prefix)) == 0)
	{
		__PRINTINFO("ͨ��forkͬʱ�������gs����ʱ�������servername��Ӧ�ķ������������һ����ƥ��\n");
		return -1;
	}
	
	__PRINTINFO("��ʼ�� %s\n",servername);

//	setenv("MALLOC_TRACE", "/tmp/m.log", 1);
//	mtrace();
	if(int rst = InitWorld(gmconf_file,servername))
	{
		__PRINTINFO("��ʼ��ʧ�ܣ� ����ţ�%d\n",rst);
		_exit(rst);
		return rst;
	}

	fflush(stdout);
	int header_offset = ftell(stdout);
	if(header_offset <=0 || header_offset > 0x500000) 
	{
		header_offset = 2048;
	}

	//����thread
	Conf *conf = ONET::Conf::GetInstance();
	int logic_threads = atoi(conf->find("General","logic_threads").c_str());
	if(logic_threads < 1 ) logic_threads = 1 ;
	if(logic_threads > 64) logic_threads = 64;
	ONET::Thread::Pool::CreatePool(logic_threads);
	ONET::Thread::Pool::CreateThread(GMSV::StartPollIO);
	int log_counter = 0;

	if(__PRINT_INFO_FLAG) ThreadUsage::Start(60, 0);
	
	setlinebuf(stdout);

	
		LuaManager::GetInstance()->InitWorld();
		LotteryConfig::GetInstance()->Init();
		TreasureConfig::GetInstance()->Init();
		ArenaConfig::GetInstance()->Init();
		GlyphConfig::GetInstance()->Init();
		WorldPointsConfig::GetInstance()->Init();
		ActivityEventConfig::GetInstance()->Init();
		CelestialMemorialConfig::GetInstance()->Init();
		EmulateSettings::GetInstance()->Init();
		CodexConfig::GetInstance()->Init();
	

	while(1) 
	{
		sleep(40);
		log_counter += 40;
		if(log_counter > 240)
		{
			log_counter -= 240;
			GLog::log(GLOG_INFO,"gameserver %d , �û���Ŀ:%d", world_manager::GetWorldIndex(),world_manager::GetInstance()->GetOnlineUserNumber());
		}
		time_t ct = time(NULL);
		__PRINTINFO("current time:%s",ctime(&ct));
		if(__PRINT_INFO_FLAG) ThreadUsage::Dump(stdout);
		__PRINTINFO("tick:%d, thread_lock_count(60sec):%d exec_task_count:%d cur_task_count:%d\n",
				g_timer.get_tick(),ONET::__thread_lock_count, 
				ONET::__thread_task_count, ONET::Thread::Pool::QueueSize());
		__PRINTINFO("free timer count:%8d total alloced:%d\n",g_timer.get_free_timer_count(),g_timer.get_timer_total_alloced());
		__PRINTINFO("total threadpool task count:%d\n",g_timer.get_tick(),ONET::Thread::Pool::GetTotalTaskCount());
		if(__PRINT_INFO_FLAG) abase::fast_allocator::dump(stdout);
		__PRINTINFO("Large size memory block:\n");
		if(__PRINT_INFO_FLAG) abase::fast_allocator::dump_large(stdout);
		__PRINTINFO("_________________________________________________________________________\n");
		world * pPlane = world_manager::GetInstance()->GetWorldByIndex(0);
		if(pPlane)
		{
			pPlane->DumpMessageCount();
		}
		
		__PRINTINFO("cur_player_count %d\n",world_manager::GetInstance()->GetOnlineUserNumber());
		__PRINTINFO("_________________________________________________________\n");
		__PRINTINFO("total_size %d\n",total_mem_size);
		__PRINTINFO("large counter:%d\n",large_counter);
		/*unsigned int index = 0;
		for(unsigned int i = 0; i < CSIZE; i ++)
		{
			if(size_counter[i])
			{
				index ++;
				__PRINTINFO("%5d:%8d  count %5d\n",index, i, size_counter[i]);
			}
		}*/
		//��ӡ����ķ�ʱ���¼
		//if(log_counter < 60 )
		{
			extern unsigned long long cmd_rdtsc_counter[1024];
			extern unsigned long long cmd_number_counter[1024];
			for(unsigned int i = 0; i < 1024; i ++)
			{
				unsigned long long n1 = cmd_number_counter[i];
				unsigned long long n2 = cmd_rdtsc_counter[i];
				if(n2 == 0) continue;
				__PRINTINFO("cmd:%3d  %8d times average use: %6d\n",i, (int)n1, (int)(n2/n1));
			}
			__PRINTINFO("_________________________________________________________\n");
		}
		{
			for(unsigned int i = 0; i < 1024; i ++)
			{
				unsigned long long n1 = GMSV::s2c_cmd_number_counter1[i];
				unsigned long long n2 = GMSV::s2c_cmd_number_counter2[i];
				if(n1 == 0) continue;
				__PRINTINFO("s2c cmd:%3d  counter1 %lld counter2 %lld\n",i, n1, n2);
			}
			__PRINTINFO("_________________________________________________________\n");
		}

		
		int pos = ftell(stdout);
		if(pos >= 0)
		{
			__PRINTINFO("cur_file_size %u\n",pos);
			fflush(stdout);
			if(pos > 0x5FFFFFFF)
			{
				//���ǽ��ж�stdout���м���
				//����ͷ�����������ݵ�
				ftruncate(fileno(stdout),header_offset);
				fseek(stdout,header_offset,SEEK_SET);
			}
		}
		else
		{
			fflush(stdout);
		}

		world_manager::GetGlobalController().CheckUpdate();
	}
	return 0;
}



