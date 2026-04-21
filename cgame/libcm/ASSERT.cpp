#include <stdio.h>
#include "ASSERT.h"

int *ASSERT_FAIL(const char * msg, const char *filename , int line)
{
	fprintf(stdout,"assert failed:\"%s\" in file %s:%d\n",msg,filename,line);
	fflush(NULL);
	return NULL;
}
