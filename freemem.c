#include "types.h" 
#include "stat.h" 
#include "fcntl.h"
#include "user.h"
#include "param.h"

int 
main(int argc, char * argv[])
{
	printf(1, "current length of freelist of kernel: %d\n", freemem());
	exit();
	return 0;
}
