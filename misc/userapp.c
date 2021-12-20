#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
int main()
{
	int fd;
	char buffer[50]="hello shashank parsi";
	fd=open("/dev/misc-test-driver",O_RDWR);
	if(fd < 0)
	{
		printf("misc-test-driver open failed\n");
		return -1;
	}
	int len=write(fd,buffer,sizeof(buffer));
	printf("buffer data: %s\n",buffer);
	printf("written : %d\n",len);
	lseek(fd,0,SEEK_SET);
	memset(buffer,0,sizeof(buffer));
	len=read(fd,buffer,sizeof(buffer));
	printf("readed: %d\n",len);
	printf("buffer data: %s\n",buffer);
	close(fd);
	return 0;
}
