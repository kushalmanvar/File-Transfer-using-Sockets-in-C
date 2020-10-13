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

int checkConnection(int);
int completeTransfer(int);

int main(int argc, char *argv[])
{
	
	int sockfd, portno, n;
	//socklen_t 
	
	char buffer[255];
	struct hostent *server;
	struct sockaddr_in serv_addr, cli_addr;	//server address - client address
	socklen_t clilen;	//client addr length
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	if(argc < 3){
		printf("Please provide a valid host and port no:");
		exit(1);
	}
	
	if(sockfd < 0)
	{
		
		printf("Error opening socket");
	}
	
	server = gethostbyname(argv[1]);
	portno = atoi(argv[2]);
	if(server == NULL)
	{
		printf("Please provide a valid host\n");
	}
	
	bzero((char *) &serv_addr, sizeof(serv_addr));
	
	serv_addr.sin_family = AF_INET;
	bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
	
	serv_addr.sin_port = htons(portno);
	
	if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("Connection failed\n");
	}
	else{
		printf("Connection to server established\n");
	}
	
	
	
	
	while(1)
	{
		
		bzero(buffer, 255);
		char *input[100],client[110];
		char inputs[10][100];
		char filename[100];
		int ctr=0,j=0;
		int x,n;
		char ch = 'A', c = 'A';
		FILE *fp;
		int get = 1,put = 1;
		int words = 0,w=0;
		int status = 0;
		char *exists = "N";
		int i = 0;
		char s[100];
		//checkConnection(sockfd);
		sleep(1);
		//printf("Current working directory: %s\n", getcwd(s, 100)); 
		printf("Plese choose from one of the following:\n");
		printf("get fileName , put fileName, quit\n");
		
		fgets(client,255,stdin);
		
		strcpy(buffer,client);
		//printf("buffer: %s\n", buffer);
		write(sockfd, buffer, 255);
		printf("Writing: %s",buffer);
		x = strncmp("quit", client, 4);
		printf("%d\n",x);
		
		if(x == 0)
		{
			printf("Closing connection\n");
			break;
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
				inputs[ctr][j]=buffer[i];
				j++;
			}
		}
		
		/*char *ptr  = strtok(client, " ");
		while(ptr != NULL)
		{
			input[i] = ptr;
			i++;
			ptr = strtok(NULL, " ");
		}*/
		bzero(buffer, 255);
		printf("Command: %s\n", inputs[0]);
		strcpy(filename,inputs[1]);
		printf("Filename: %s\n", filename);
		
		get = strncmp("get", inputs[0], 3);
		put = strncmp("put", inputs[0], 3);
		
		//printf("get: %d\n", get);
		//printf("put: %d\n", put);
		
		printf("");
		
		sleep(1);
		
		if(get == 0)
		{
			//printf("In get\n");
			/*n = read(sockfd, buffer, 255);
			if(n < 0)
			{
				printf("Error while reading input\n");
				continue;
			}
			
			n = strncmp(exists,buffer,1);
			if(n == 0)
			{
				printf("Server doesnt have the required file\n");
				continue;
			}
			else
			{
				printf("Server is sending the file\n");
			}*/

			
			fp = fopen("server_file.txt", "w");
			
			if(fp == NULL)
			{
				printf("Issue while opening file\n");
				continue;
			}
			words = 0;
			read(sockfd, &words, sizeof(int));
			bzero(buffer, 255);
			printf("", words);
			printf("File Received\n");
			
			while(w != words)
			{
				read(sockfd, buffer, 255);
				fprintf(fp, "%s ", buffer);
				fflush(fp);
				printf("");
				bzero(buffer, 255);
				w++;
			}
			bzero(buffer, 255);
			fclose(fp);
			//checkConnection(sockfd);

		}
		else if(put == 0)
		{
			//printf("In put\n");
			fp = fopen(filename, "r");
			if(fp == NULL)
			{
				printf("Please provide a valid filename\n");
				//exists = "N";
				//write(sockfd, exists, 255);
				continue;
			}
			else
			{
				//exists = "Y";
				//write(sockfd, exists, 255);
				words = 0,w=0;
				bzero(buffer, 255);
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
				write(sockfd, &words, sizeof(int));
				rewind(fp);
				
				printf("File Send\n");
				while(w != words)
				{
					fscanf(fp, "%s", buffer);
					printf("");
					write(sockfd, buffer, 255);
					ch = fgetc(fp);
					bzero(buffer, 255);
					w++;
				}
				bzero(buffer, 255);
			}

			fclose(fp);
			
			/*status = completeTransfer(sockfd);
			if(status == 1)
			{
				printf("File successfully transferred to server\n");
				printf("status: %d",status);
			}
			else
			{
				printf("status: %d",status);
			}*/
			
			
		}
		else{
			printf("Invalid command\n");
		}
		
	}
	printf("Exiting\n");
	close(sockfd);
	return 0;
	
}

int completeTransfer(int sockfd)
{
	int ready,n;
	sleep(1);
	n = read(sockfd, &ready, sizeof(int));
		
	if(ready == 1)
	{
		printf("Server is ready: %d\n", ready);
		return 1;
	}
	else{
		return 0;
	}
}

int checkConnection(int sockfd)
{
	int ready = 1;
	sleep(1);	
	write(sockfd, &ready, sizeof(int));
	
	return 1;
}