#define _BSD_SOURCE
#define _XOPEN_SOURCE 500
#include <assert.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "dma_utils.c"
#define DEVICE_NAME_DEFAULT "/dev/xdma0_c2h_0"
#define DEVICE_WRITE "/dev/xdma0_h2c_0"
#define SIZE_DEFAULT (4)
#define COUNT_DEFAULT (1)
static int test_dma(char *devname, uint64_t addr, uint64_t size,
		    uint64_t offset, uint64_t count, char *ofname);
static int no_write = 0;
ssize_t send_command(char* header, char* command1, char* command2, char* command3);
void read_cr();
int main ()
{	
	/*uint64_t address = 0;
	uint64_t size = SIZE_DEFAULT;
	uint64_t offset = 0;
	uint64_t count = COUNT_DEFAULT;
	char *ofname = NULL;
	test_dma(device, address, size, offset, count, ofname);*/
	char *allocated_header=NULL;
	char *header=NULL;
	char *allocated_command1=NULL;
	char *command1=NULL;
	char *allocated_command2=NULL;
	char *command2=NULL;
	char *allocated_command3=NULL;
	char *command3=NULL;
	printf("comando 2\n");
	posix_memalign((void **)&allocated_header, 4096 /*alignment */ , 4+4096);
	if (!allocated_header) {
		fprintf(stderr, "OOM %lu.\n", 8 + 4096);
		
	}
	posix_memalign((void **)&allocated_command1, 4096 /*alignment */ , 8+4096);
	if (!allocated_command1) {
		fprintf(stderr, "OOM %lu.\n", 8 + 4096);
		
	}
	posix_memalign((void **)&allocated_command2, 4096 /*alignment */ , 12+4096);
	if (!allocated_command2) {
		fprintf(stderr, "OOM %lu.\n", 8 + 4096);
		
	}
	posix_memalign((void **)&allocated_command3, 4096 /*alignment */ , 16+4096);
	if (!allocated_command3) {
		fprintf(stderr, "OOM %lu.\n", 8 + 4096);
		
	}
	header = malloc(sizeof(char)*4);
	command1 = malloc(sizeof(char)*4);	
	command2 = malloc(sizeof(char)*4);
	command3 = malloc(sizeof(char)*4);
	
	// Thread stop
	header[0]=0x00; //i primi 4 byte sono il numero di comandi da dare al router
	header[1]=0x02; //i secondi 4 byte indica a chi è diretto il comando 0 per memmory
	header[2]=0x00; //controller 1 per nuplus
	header[3]=0x01;
	
	
	command1[0]=0x00; //comando che abilita/disabilita thread
	command1[1]=0x00;
	command1[2]=0x00;
	command1[3]=0x01;

	
	command2[0]=0x00; //maschere cha abilita è disabilita i thread
	command2[1]=0x00;
	command2[2]=0x00;
	command2[3]=0x00;

	
	send_command( header, command1, command2, NULL);	
	// SET PC
	
	header[0]=0x00;
	header[1]=0x03;
	header[2]=0x00;
	header[3]=0x01;
	
	
	command1[0]=0x00; //comando che setta il PC
	command1[1]=0x00; 
	command1[2]=0x00;
	command1[3]=0x00;

	
	command2[0]=0x00; //thread di cui settare il PC
	command2[1]=0x00;
	command2[2]=0x00;
	command2[3]=0x00;


	command3[0]=0x20; //valore PC
	command3[1]=0x00;
	command3[2]=0x04;
	command3[3]=0x00;
	send_command( header,  command1, command2, command3);
	
	//dopo aver dato l' indirizzo devo dare attivare i thread nuplus se l'aspetta
	//EN THREAD
	
	header[0]=0x00;
	header[1]=0x02;
	header[2]=0x00;
	header[3]=0x01;
	
	
	command1[0]=0x00;
	command1[1]=0x00;
	command1[2]=0x00;
	command1[3]=0x01;

	
	command2[0]=0x00;
	command2[1]=0x00; //abilito un thread quello 0
	command2[2]=0x00;
	command2[3]=0x01;


	
	
	send_command( header, command1, command2, NULL);
	
	//READ REG b
	
	header[0]=0x00;
	header[1]=0x02;
	header[2]=0x00;
	header[3]=0x01;
	
	
	command1[0]=0x00; //comando per leggere registri di stato
	command1[1]=0x00;
	command1[2]=0x00;
	command1[3]=0x02;
	
	command2[0]=0x00; //i primi 4 byte indicando di quale thread si vuole
	command2[1]=0x00; //leggere il registro i secondi byte il registro
	command2[2]=0x00; 
	command2[3]=0x0b;

	
	
	//send_command( header,  command1, command2, NULL);
	//READ REG b
	
	header[0]=0x00;
	header[1]=0x02;
	header[2]=0x00;
	header[3]=0x01;
	
	
	command1[0]=0x00;
	command1[1]=0x00;
	command1[2]=0x00;
	command1[3]=0x02;
	
	command2[0]=0x00;
	command2[1]=0x00;
	command2[2]=0x00;
	command2[3]=0x0b;

	
	command3[0]=0x00;
	
	//send_command( header,  command1, command2, NULL);
	//READ REG 14
	
	header[0]=0x00;
	header[1]=0x02;
	header[2]=0x00;
	header[3]=0x01;
	
	
	command1[0]=0x00;
	command1[1]=0x00;
	command1[2]=0x00;
	command1[3]=0x02;
	
	command2[0]=0x00;
	command2[1]=0x00;
	command2[2]=0x00;
	command2[3]=0x14;

	
	
	//send_command( header,  command1, command2, NULL);
	
	//MEMREAD
	header[0]=0x00; 
	header[1]=0x02;
	header[2]=0x00;
	header[3]=0x00;
	
	
	command1[0]=0x00; //quante word voglio leggere dalla memoria
	command1[1]=0x00;
	command1[2]=0x00;
	command1[3]=0xFF;
	
	command2[0]=0x20;  //a partire da quale indirizzo si vuole 
	command2[1]=0x00;  //iniziare a leggere
	command2[2]=0x04;
	command2[3]=0x00;

	
	
	//send_command( header,  command1, command2, NULL);

	free(header);
	free(command1);
	free(command2);
	free(command3);
}

ssize_t send_command(char* header, char* command1, char* command2, char* command3)
{
	printf("header \n");
	ssize_t rc;
	char *buffer = NULL;
	char *allocated = NULL;
	char *old_buffer = NULL;
	char *old_allocated = NULL;
	int value;
	char *devr = DEVICE_NAME_DEFAULT;
	int fpga_fd = open(devr, O_RDWR | O_NONBLOCK);
	char * devw=DEVICE_WRITE;
	int fpga_fdw = open(devw, O_RDWR);
	char *allocated_null = NULL;
	posix_memalign((void **)&allocated_null, 4096 /*alignment */ , 20+4096);
	if (!allocated_null) {
		fprintf(stderr, "OOM %lu.\n", 4 + 4096);
		
	}
	posix_memalign((void **)&old_allocated, 4096 /*alignment */ , 24+4096);
	if (!old_allocated) {
		fprintf(stderr, "OOM %lu.\n", 4 + 4096);
		
	}
	old_buffer=malloc(sizeof(char)*4);
	char *null_command=malloc(sizeof(char)*4);
	null_command[0]=0x00;
	null_command[1]=0x00;
	null_command[2]=0x00;
	null_command[3]=0x00;
	
	if (fpga_fd < 0) {
                fprintf(stderr, "unable to open device %s, %d.\n",
                        devr, fpga_fd);
		perror("open device");
                return -EINVAL;
        }
	if (fpga_fdw < 0) {
                fprintf(stderr, "unable to open write device %s, %d.\n",
                        devw, fpga_fdw);
		perror("open device");
                return -EINVAL;
        }

	posix_memalign((void **)&allocated, 4096 /*alignment */ , 28+4096);
	if (!allocated) {
		fprintf(stderr, "OOM %lu.\n", 8 + 4096);
		rc = -ENOMEM;
		goto out;
	}

	buffer = malloc(sizeof(char)*4);
	printf("header \n");
	int command_number=header[1];
	
	write_from_buffer_swapped(devw, fpga_fdw, header, 4, 0);
	//sleep(5);
	null_command[3]=0x01;
	write_from_buffer_swapped(devw, fpga_fdw, null_command, 4, 4);
	null_command[0]=0x00;
	
	
	printf("comando 1\n");
	
	write_from_buffer_swapped(devw, fpga_fdw, null_command, 4, 4);
	sleep(1);
	write_from_buffer_swapped(devw, fpga_fdw, command1, 4, 0);
	null_command[3]=0x01;
	write_from_buffer_swapped(devw, fpga_fdw, null_command, 4, 4);
	null_command[0]=0x00;
	sleep(1);
	
	if (command2!=NULL)
	{
		printf("comando 2\n");
		
		write_from_buffer_swapped(devw, fpga_fdw, null_command, 4, 4);
		rc = read_to_buffer(devr, fpga_fd, buffer, 4, 12);
		printf("aspetto 0x%2x\n",buffer[1]);
		for (int i=0;i<4;i++)
		{
			printf("buffer %d %02x \n", i,buffer[i]);
			value=(buffer[i]<<8*i)|value;

		}
		printf ("buf %08x \n",value);
		sleep(1);
		while ((buffer[1]&0x02)==1)
		{
			printf("aspetto 0x%2x\n",buffer[1]);
			rc = read_to_buffer(devr, fpga_fd, buffer, 4, 12);
		}
		write_from_buffer_swapped(devw, fpga_fdw, command2, 4, 0);
		null_command[3]=0x01;
		write_from_buffer_swapped(devw, fpga_fdw, null_command, 4, 4);
		null_command[0]=0x00;
		sleep(1);
		
		
	}
	if (command3!=NULL)
	{
		printf("comando 3\n");
		
		write_from_buffer_swapped(devw, fpga_fdw, null_command, 4, 4);
		write_from_buffer_swapped(devw, fpga_fdw, command3, 4, 0);
		null_command[3]=0x01;
		write_from_buffer_swapped(devw, fpga_fdw, null_command, 4, 4);
		null_command[0]=0x00;
		sleep(1);
		
		
	}
	write_from_buffer_swapped(devw, fpga_fdw, null_command, 4, 4);
	rc = read_to_buffer(devr, fpga_fd, buffer, 4, 12);
	free(allocated_null);	
	if (rc < 0)
		goto out;
	for (int i=0;i<4;i++)
		{
			//printf("buffer %d %02x", i,buffer[i]);
			value=(buffer[i]<<8*i)|value;

		}
		printf ("buf %08x",value);
	rc = 0;
out:
	close(fpga_fd);
	close(fpga_fdw);
	free(allocated);
	return rc;
}
//read mem
		/*bufferw[0]=0x00;
		bufferw[1]=0x00;
		bufferw[2]=0x02;
		bufferw[3]=0x00;
		bufferw[4]=0x00;
		bufferw[5]=0x00;
		bufferw[6]=0x00;
		bufferw[7]=0x00;
		//printf("scrivo 0x0100010001000000");
		rc = write_from_buffer_swapped(dev, fpga_fdw, bufferw, 8, addr);
		if (rc < 0)
			goto out;
		bufferw[0]=0x00;
		bufferw[1]=0x00;
		bufferw[2]=0x02;
		bufferw[3]=0x00;
		bufferw[4]=0x01;
		bufferw[5]=0x00;
		bufferw[6]=0x00;
		bufferw[7]=0x00;
		//printf("scrivo 0x0100010001000000");
		rc = write_from_buffer_swapped(dev, fpga_fdw, bufferw, 8, addr);
		if (rc < 0)
			goto out;
		printf ("rc %d\n",rc); 
		bufferw[0]=0x00;
		bufferw[1]=0x00;
		bufferw[2]=0x00;
		bufferw[3]=0x00;
		bufferw[4]=0x00;
		bufferw[5]=0x00;
		bufferw[6]=0x00;
		bufferw[7]=0x00;
		//printf("scrivo 0x0200000001000000");
		rc = write_from_buffer_swapped(dev, fpga_fdw, bufferw, 4, 4);
		if (rc < 0)
			goto out;
		bufferw[0]=0x02;
		bufferw[1]=0x00;
		bufferw[2]=0x00;
		bufferw[3]=0x00;
		bufferw[4]=0x01;
		bufferw[5]=0x00;
		bufferw[6]=0x00;
		bufferw[7]=0x00;
		//printf("scrivo 0x0200000001000000");
		rc = write_from_buffer_swapped(dev, fpga_fdw, bufferw, 8, 0);
		if (rc < 0)
			goto out;
		bufferw[0]=0x00;
		bufferw[1]=0x00;
		bufferw[2]=0x00;
		bufferw[3]=0x00;
		bufferw[4]=0x00;
		bufferw[5]=0x00;
		bufferw[6]=0x00;
		bufferw[7]=0x00;
		//printf("scrivo 0x0200000001000000");
		rc = write_from_buffer_swapped(dev, fpga_fdw, bufferw, 4, 4);
		if (rc < 0)
			goto out;
		bufferw[0]=0x00;
		bufferw[1]=0x00;
		bufferw[2]=0x00;
		bufferw[3]=0x10;
		bufferw[4]=0x01;
		bufferw[5]=0x00;
		bufferw[6]=0x00;
		bufferw[7]=0x00;
		//printf("scrivo 0x0200000001000000");
		rc = write_from_buffer_swapped(dev, fpga_fdw, bufferw, 8, addr);
		/*if (rc < 0)
			goto out;
		bufferw[0]=0x00;
		bufferw[1]=0x00;
		bufferw[2]=0x00;
		bufferw[3]=0x00;
		bufferw[4]=0x00;
		bufferw[5]=0x00;
		bufferw[6]=0x00;
		bufferw[7]=0x00;
		//printf("scrivo 0x0200000001000000");
		rc = write_from_buffer_swapped(dev, fpga_fdw, bufferw, 4, 4);
		if (rc < 0)
			goto out;
		bufferw[0]=0x00;
		bufferw[1]=0x00;
		bufferw[2]=0x00;
		bufferw[3]=0x00;
		bufferw[4]=0x01;
		bufferw[5]=0x00;
		bufferw[6]=0x00;
		bufferw[7]=0x00;
		//printf("scrivo 0x0200000001000000");
		rc = write_from_buffer_swapped(dev, fpga_fdw, bufferw, 8, addr);*/

