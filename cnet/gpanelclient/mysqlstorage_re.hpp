
#ifndef __GNET_MYSQLSTORAGE_RE_HPP
#define __GNET_MYSQLSTORAGE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gmysqlrow"

namespace GNET
{

class MySQLStorage_Re : public GNET::Protocol
{
	#include "mysqlstorage_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		
		
		printf("MYSQL::LOG:Input: ONPUT_COUNT = %d \n", output_str.size() );
		for (size_t i = 0; i < output_str.size(); i++)
		{
			printf("MYSQL::LOG:Input: ONPUT_POS = %d , ROW_COUNT = %d \n", i , output_str[i].str.size() );
			for (size_t j = 0; j < output_str[i].str.size(); j++)
			{
				std::string out((const char*)output_str[i].str[j].begin(), output_str[i].str[j].size());
				printf("MYSQL::LOG:Output: iIndex = %d , oPos=%d , rPos=%d , str = %s \n", output_str[i].iindex, i, j, out.c_str() );
			}
		}
		
		
		
		
		
		
		printf("MySQLStorage_Re::Process: --OK-- \n");
	}
};

};

#endif
