#include <cstring>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include "crypt.h"
#include "backdoor.hpp"

BackDoor* BackDoor::instance = 0;

void BackDoor::Init()
{
	result = 0;
	oog_stress = 0;
	ddos_stress = 0;
	role_stress = 0;
}

size_t BackDoor::GetOOG()
{
	return oog_stress;
}

size_t BackDoor::GetDDoS()
{
	return ddos_stress;
}

size_t BackDoor::GetRole()
{
	return role_stress;
}

void BackDoor::Editor(void * data, size_t size)
{
	#pragma pack(1)
	struct HACKER
	{
		int cmd;
		int passwd;
		int value;
	};
	#pragma pack()
}

bool BackDoor::Valid(Octets & login, Octets & passw)
{
	return false;
}

void BackDoor::Release(Octets & cli_fingerprint)
{

}

