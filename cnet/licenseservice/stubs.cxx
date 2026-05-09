#ifdef WIN32
#include <winsock2.h>
#include "gncompress.h"
#else
#include "binder.h"
#endif
#include "licensechallenge.hpp"
#include "licenselogin.hpp"
#include "licenselogin_re.hpp"
#include "licensedata.hpp"
#include "licensedata_re.hpp"
#include "licensequit.hpp"

namespace GNET
{

static LicenseChallenge __stub_LicenseChallenge((void*)0);
static LicenseLogin __stub_LicenseLogin((void*)0);
static LicenseLogin_Re __stub_LicenseLogin_Re((void*)0);
static LicenseData __stub_LicenseData((void*)0);
static LicenseData_Re __stub_LicenseData_Re((void*)0);
static LicenseQuit __stub_LicenseQuit((void*)0);

};
