#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio_ext.h>


int serviceClient(int, char *);
int checkConnection(int);
int completeTransfer(int);

int main(int argc, char *argv[])
{
	
	int pid;
	int sockfd, newsockfd, portno, n;
	int status;
	//socklen_t 
	
	char buffer[255];
	
	struct sockaddr_in serv_addr, cli_addr;	//server address - client address
	socklen_t clilen;	//client addr length
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	if(argc < 2)
	{
		printf("Please provide a port no for the server\n");
		exit(1);
	}
	
	if(sockfd < 0)
	{
		printf("Error opening socket");
	}
	
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);		//host to network short
	
	if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("Binding failed");	
	}
	
	listen(sockfd, 5);
	clilen = sizeof(cli_addr);
	
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	
	if(newsockfd < 0)
	{
		printf("Error in accept");
	}
	else
	{
		printf("Connection to client established\n");;
		if(fork() == 0)
		{
			//printf("Creating child process");
			int n = serviceClient(newsockfd,buffer);
			
			if(n == 1)
			{
				close(newsockfd);
				close(sockfd);
				return 1;
			}
		}
		else
		{
			printf("Parent process is waiting");
			wait(&status);
		}
	}
	
	close(newsockfd);
	close(sockfd);
	return 1;
}

int serviceClient(int newsockfd, char buffer[255])
{
	
	printf("Child process created\n");
	
	while(1)
	{
		bzero(buffer, 255);
		FILE *fp;
		char ch = 'A',c = 'A';
		int words = 0,w = 0;
		int get = 1,put = 1;
		int ctr=0,j=0;
		int x,n;
		char *input[100];
		char inputs[10][100];
		char command[100];
		char filename[100];
		char *exists = "N";
		int status = 0;
		char s[100];
		int count = 0;
		int i = 0;
		/*status = checkConnection(newsockfd);
		
		if(status != 1)
		{
			printf("Issue in connection");
			continue;
		}*/
		sleep(2);
		printf("Current working directory: %s\n", getcwd(s, 100)); 
		printf("Waiting for client input:\n");
		n = read(newsockfd, buffer, 255);
		strcpy(command,buffer);
		//printf("%d",n);
		if(n < 0)
		{
			printf("Error while reading input\n Waiting for next input:\n");
			continue;
		}
		printf("Command from client: %s\n", command);
		
		x = strncmp("quit", command, 4);
		//printf("%d",x);
		
		if(x == 0)
		{
			printf("Client sent quit\n");
			printf("Closing connection\n");
			return 1;
		}
		
		
		for(int i=0;i<=(strlen(buffer));i++)
		{
			// if space or NULL found, assign NULL into newString[ctr]
			if(buffer[i]== ' ' || buffer[i]=='\0' || buffer[i]=='\n')
			{
				inputs[ctr][j]='\0';
				ctr++;  //for next word
				j=0;    //for next word, init index to 0
			}
			else
			{
				inputs[ctr][j]= buffer[i];
				j++;
			}
			
			
		}
		
		/*char *ptr = strtok(command, " ");
		while(ptr != NULL)
		{
			input[i] = ptr;
			i++;
			ptr = strtok(NULL, " ");
		}*/
		
		
		bzero(buffer, 255);
		
		printf("Command:%s\n", inputs[0]);
		strcpy(filename,inputs[1]);
		printf("Filename:%s\n", filename);
		
		get = strncmp("get", inputs[0], 3);
		put = strncmp("put", inputs[0], 3);
		printf("");
		printf("");
		printf("");
		
		sleep(1);
		
		if(get == 0)
		{
			printf("");
			fp = fopen(filename, "r");
			if(fp == NULL)
			{
				printf("File doesn't exist\n");
				//exists = "N";
				//write(newsockfd, exists, 255);
				continue;
			}
			else
			{
				//exists = "Y";
				//write(newsockfd, exists, 255);
				words = 0, w = 0;
				while((c=getc(fp)) != EOF)
				{
					fscanf(fp, "%s", buffer);
					if(isspace(c) || c == '\t')
						words++;
				}
				if(words != 0)
				{
					words++;
				}
				printf("");
				write(newsockfd, &words, sizeof(int));
				rewind(fp);
				printf("File Send..\n");
				bzero(buffer, 255);
				while(w != words)
				{
					fscanf(fp, "%s", buffer);
					printf("");
					write(newsockfd, buffer, 255);
					bzero(buffer, 255);
					ch = fgetc(fp);
					w++;
				}
				
			}

			fclose(fp);
			bzero(buffer, 255);
			/*status = completeTransfer(newsockfd);
			if(status == 1)
			{
				printf("File successfully transferred to client machine\n");
				printf("status: %d",status);
			}
			else
			{
				printf("status: %d",status);
			}*/
			
		}
		else if(put == 0)
		{
			printf("");
			/*n = read(newsockfd, buffer, 255);
			if(n < 0)
			{
				printf("Error while reading input\n");
				continue;
			}
			
			n = strncmp(exists,buffer,1);
			if(n == 0)
			{
				printf("Client didn't provide a valid file\n");
				continue;
			}
			else
			{
				printf("Client is sending the file\n");
			}*/
			
			fp = fopen("client_file.txt", "w");
			
			if(fp == NULL)
			{
				printf("Issue while opening file\n");
				continue;
			}
			words = 0;
			read(newsockfd, &words, sizeof(int));
			
			printf("");
			printf("File Received\n");
			bzero(buffer, 255);
			while(w != words)
			{
				read(newsockfd, buffer, 255);
				fprintf(fp, "%s ", buffer);
				fflush(fp);
				printf("");
				bzero(buffer, 255);
				w++;
			}
			bzero(buffer, 255);
			fflush(fp);
			fclose(fp);
			//checkConnection(newsockfd);
			
			
		}
		else
		{
			printf("Invalid input from client\n");
		}
		
	}
	
	return 1;
}

int completeTransfer(int newsockfd)
{
	int ready,n;
	sleep(1);
	n = read(newsockfd, &ready, sizeof(int));
		
	if(ready == 1)
	{
		printf("Client is ready: %d\n", ready);
		return 1;
	}
	else{
		return 0;
	}
	
}

int checkConnection(int newsockfd)
{
	int ready = 1;
	sleep(1);
	write(newsockfd, &ready, sizeof(int));
	
	return 1;
}
