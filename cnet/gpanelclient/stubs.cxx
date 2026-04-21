#ifdef WIN32
#include <winsock2.h>
#include "gncompress.h"
#else
#include "binder.h"
#endif
#include "gmysqlstorage.hrp"
#include "panelchallenge.hpp"
#include "panelresponse.hpp"
#include "panelresponse_re.hpp"
#include "mysqlstorage.hpp"
#include "mysqlstorage_re.hpp"

namespace GNET
{

static GMySQLStorage __stub_GMySQLStorage (RPC_GMYSQLSTORAGE, new GMySQLStorageArg, new GMySQLStorageRes);
static PanelChallenge __stub_PanelChallenge((void*)0);
static PanelResponse __stub_PanelResponse((void*)0);
static PanelResponse_Re __stub_PanelResponse_Re((void*)0);
static MySQLStorage __stub_MySQLStorage((void*)0);
static MySQLStorage_Re __stub_MySQLStorage_Re((void*)0);

};
