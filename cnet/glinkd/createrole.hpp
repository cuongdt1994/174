
#ifndef __GNET_CREATEROLE_HPP
#define __GNET_CREATEROLE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "glinkserver.hpp"
#include "gdeliveryclient.hpp"
#include "groleinventory"
#include "roleinfo"
namespace GNET
{

class CreateRole : public GNET::Protocol
{
	#include "createrole"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
		if (!GLinkServer::GetInstance()->ValidUser(sid,userid)) return; //--валидный юзер
		int _gender = roleinfo.gender;
		int _coccup = roleinfo.occupation;
		int _cssize = roleinfo.custom_data.size();
		if (_gender > 1u  || _coccup > 15u )  return; //--пол, класс, 
		if (_gender == 0  && _coccup == 3  )  return; //--Дру Муж
		if (_gender == 1  && _coccup == 4  )  return; //--Обор Жен
		if (_cssize < 176 || _cssize > 1024)  return; //--Внешка
		
		/*
		if (_cssize >= 200)
		{
			roleinfo.hd_custom_data = roleinfo.custom_data;
		}
		*/
		this->localsid=sid;
		if ( GDeliveryClient::GetInstance()->SendProtocol(this) )
		{
			GLinkServer::GetInstance()->ChangeState(sid,&state_GCreateRoleReceive);
		}
		else
			GLinkServer::GetInstance()->SessionError(sid,ERR_COMMUNICATION,"Server Network Error.");
	}
};

};

#endif
