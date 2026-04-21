
#ifndef __GNET_PLAYERLOGIN_HPP
#define __GNET_PLAYERLOGIN_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "localmacro.h"
#include "gdeliveryserver.hpp"
#include "mapuser.h"
#include "crosssystem.h"

namespace GNET
{

class PlayerLogin : public GNET::Protocol
{
	#include "playerlogin"
	void SendFailResult(GDeliveryServer* dsm,Manager::Session::ID sid,int retcode);
	void SendForbidInfo(GDeliveryServer* dsm,Manager::Session::ID sid,const GRoleForbid& forbid);
	bool PermitLogin(GDeliveryServer* dsm,Manager::Session::ID sid);
	void SetGMPrivilege( int roleid );
	void DoLogin(Manager::Session::ID sid, UserInfo* pUser, bool is_central);
	void TryRemoteLogin(Manager::Session::ID sid, UserInfo* pUser);

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("gdelivery::receive playerlogin from link,roleid=%d,linkid=%d,localsid=%d\n",
			roleid,provider_link_id,localsid);
		GDeliveryServer* dsm = GDeliveryServer::GetInstance();
		
		int userid = UidConverter::Instance().Roleid2Uid(roleid);
		UserInfo* pUser = UserContainer::GetInstance().FindUser(userid);
		if( (pUser == NULL) || (pUser->localsid != localsid) || (pUser->status != _STATUS_ONLINE) || (pUser->logicuid != LOGICUID(roleid)) || !pUser->rolelist.IsRoleExist(roleid) ) {
			SendFailResult(dsm, sid, ERR_LOGINFAIL);
			return;
		}

		bool is_central = dsm->IsCentralDS();
		bool is_cross_locked = pUser->IsRoleCrossLocked(roleid);
		
		if ((is_central || is_cross_locked) && pUser->is_phone)
		{
			// ʹ���ֻ���¼������������ߴ��ڿ������״̬�Ľ�ɫ
			SendFailResult(dsm, sid, ERR_LOGINFAIL);
			return;
		}

		//flag 0:������½�߼� 1:ԭ������� 2:�����ԭ�� 3:ֱ�ӵ�¼���������
		if(is_central) //����յ���Э��
		{
			//��Ϊ��ԭ��->��� flag == DS_TO_CENTRALDS����ֱ�ӵ�¼��� flag == DIRECT_TO_CENTRALDS �������
			if(flag != DS_TO_CENTRALDS && flag != DIRECT_TO_CENTRALDS) return;
		}
		else //ԭ���յ���Э��
		{
			//��Ϊ������½ԭ��flag == 0���ʹӿ���ص�ԭ��flag == CENTRALDS_TO_DS
			if(flag != CENTRALDS_TO_DS && flag != 0) return;
			//���Խ��п��ֱ�ӵ�½
			if(flag == 0 && is_cross_locked) flag = DIRECT_TO_CENTRALDS;
		}
	
		if(is_central) {
			if( !PermitLogin(dsm,sid) ) return;
			//old version
			DoLogin(sid, pUser, is_central);
		} else {
			if(flag == DIRECT_TO_CENTRALDS) {
				TryRemoteLogin(sid, pUser);
			} else {
				if( !PermitLogin(dsm,sid) ) return;
				//old version
				DoLogin(sid, pUser, is_central);
			}
		}
	}
};

};

#endif
