#include "conf.h"
#include "log.h"
#include "thread.h"
#include <iostream>
#include <unistd.h>
#include <beaktrace.h>

#include "luaman.hpp"
#include "authmanager.h"
#include "gauthserver.hpp"
#include "gmysqlclient.hpp"
#include "gpanelserver.hpp"

using namespace GNET;

int main(int argc, char *argv[])
{
	static const char * m_service = "gauthd";
	
	SetupSignalHandler();

	if (argc != 2 || access(argv[1], R_OK) == -1)
	{
		std::cerr << "Usage: " << argv[0] << " configurefile" << std::endl;
		exit(-1);
	}

	
		LuaManager::GetInstance()->Init();
		AuthManager::GetInstance()->Init();
	
	Conf *conf = Conf::GetInstance(argv[1]);
	Log::setprogname(m_service);
	{
		GAuthServer *manager = GAuthServer::GetInstance();
		manager->SetAccumulate(atoi(conf->find(manager->Identification(), "accumulate").c_str()));
		std::string value=conf->find(manager->Identification(),"shared_key");
		manager->shared_key=Octets(value.c_str(),value.size());
		Protocol::Server(manager);
	}

	{
		GPanelServer *manager = GPanelServer::GetInstance();
		manager->SetAccumulate(atoi(conf->find(manager->Identification(), "accumulate").c_str()));
		std::string value=conf->find(manager->Identification(),"shared_key");
		manager->shared_key=Octets(value.c_str(),value.size());
		std::string cltkey=conf->find(manager->Identification(),"client_key");
		manager->client_key=Octets(cltkey.c_str(),cltkey.size());
		Protocol::Server(manager);
	}

	{
		MysqlManager *manager = MysqlManager::GetInstance();		
		const char * location = "GMysqlClient";
		manager->Init 
		(
			conf->find(location, "address").c_str(),
			atoi(conf->find(location, "port").c_str()),
			conf->find(location, "user").c_str(),
			conf->find(location, "passwd").c_str(),
			conf->find(location, "name").c_str(),
			atoi(conf->find(location, "hash").c_str()) 
		);
		
		if ( !manager->Connect() )
		{
			printf("MYSQL: ERROR CONNECT!!! \n");
			exit(-1);
		}
	}
	
	
		Thread::Pool::AddTask(new LuaTimer(1));
		Thread::Pool::AddTask(new AuthTimer(1));
		Thread::Pool::AddTask(new MysqlTimer(1));
		Thread::Pool::AddTask(PollIO::Task::GetInstance());
		Thread::Pool::Run();
	
	
	
	return 0;
}

