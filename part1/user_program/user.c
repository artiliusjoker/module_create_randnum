#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
void main()
{
	int file = 0;
	int byte_read = 0;
	int receive;
	file = open("/dev/my_random", O_RDWR);
	if(file < 0){
	perror("Cannot read file");
	return;
	}
	printf("Read successfully\n");
	byte_read = read(file, &receive, sizeof(int));
	printf("Random number retrieve from my_random module : %d with bytes read : %d\n", receive, byte_read);
	return;
}
