  /* tcp_ client.c */ 
/* KSHITIJ SRIVASTAVA */
/* OCTOBER 20, 2015 */     

#include <stdio.h>          /* for standard I/O functions */
#include <stdlib.h>         /* for exit */
#include <string.h>         /* for memset, memcpy, and strlen */
#include <netdb.h>          /* for struct hostent and gethostbyname */
#include <sys/socket.h>     /* for socket, connect, send, and recv */
#include <netinet/in.h>     /* for sockaddr_in */
#include <unistd.h>         /* for close */

#define STRING_SIZE 1024

int main(void) {

   int sock_client;  /* Socket used by client */

   struct sockaddr_in server_addr;  /* Internet address structure that
                                        stores server address */
   struct hostent * server_hp;      /* Structure to store server's IP
                                        address */
   char server_hostname[STRING_SIZE]; /* Server's hostname */
   unsigned short server_port;  /* Port number used by server (remote port) */

   char sentence[STRING_SIZE];  /* send message */
   char modifiedSentence[STRING_SIZE]; /* receive message */
   unsigned int msg_len;  /* length of message */                      
   int bytes_sent, bytes_recd; /* number of bytes sent or received */
  
   /* open a socket */

   if ((sock_client = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
      perror("Client: can't open stream socket");
      exit(1);
   }

   /* Note: there is no need to initialize local client address information 
            unless you want to specify a specific local port
            (in which case, do it the same way as in udpclient.c).
            The local address initialization and binding is done automatically
            when the connect function is called later, if the socket has not
            already been bound. */

   /* initialize server address information */

   printf("Enter hostname of server: ");
   scanf("%s", server_hostname);
   if ((server_hp = gethostbyname(server_hostname)) == NULL) {
      perror("Client: invalid server hostname");
      close(sock_client);
      exit(1);
   }

   printf("Enter port number for server: ");
   scanf("%hu", &server_port);

   /* Clear server address structure and initialize with server address */
   memset(&server_addr, 0, sizeof(server_addr));
   server_addr.sin_family = AF_INET;
   memcpy((char *)&server_addr.sin_addr, server_hp->h_addr,
                                    server_hp->h_length);
   server_addr.sin_port = htons(server_port);

    /* connect to the server */
 		
   if (connect(sock_client, (struct sockaddr *) &server_addr, 
                                    sizeof (server_addr)) < 0) {
      perror("Client: can't connect to server");
      close(sock_client);
      exit(1);
   }
  
   /* user interface */
   int z = 1;
   while(z==1){    /* to make an infinte loop, so that the client does nnot terminate the connection after every connection*/
   int i,j;
	char input[10];
	char amount[10]; 
	printf(" \npress 1 to check balance followed by c or s for checking / saving account\n");
	printf (" Hint : to check balance in current account enter 1c \n");
	printf("\n press 2 to deposit money followed by c or s for checking / saving account\n");
	printf(" Hint : to deposit money in saving account enter 2s \n");
	printf("\n press 3 to withdraw money followed by c or s for checking / saving account\n");
	printf(" Hint : to withdraw money from checking account press 3c\n");
	printf(" \npress 4 to transfer money followed by c or s for checking / saving account respectively for the source account \n");
	printf(" Hint : to transfer money from current account to savings account press 4c \n");
	printf(" Hint : to transfer money from saving account to current account press 4s \n");
	printf(" \nplease enter 5 to exit \n");   

	scanf ("%s", input); /* takes input from user in the form of a string*/

	if (strstr(input, "1")!=NULL){     /* this if loop is used satisfy the check balance operation of the program*/
		msg_len = strlen(input) + 1;
		printf(" size of message : %d", msg_len );
	}

	else if ( strstr(input, "2") !=NULL) {                         /* this if loop is used satisfy the deposit operation of the program*/
		printf("please enter the amount to deposit : ");
		scanf ("%s", amount);
		for (i=0; input[i] != '\0'; ++i);                          /*for loop is used to concatenate the amount field to the end of transaction type and account type */
		for (j=0; amount[j]!= '\0'; ++j, ++i)
	{
		input[i]=amount[j];                                   
	}
		input[i]='\0';                                            /*places a null character at the end of the string*/
		msg_len=strlen(input)+1;
		printf(" size of message %d", msg_len);                 /* length of the message sent over to the server*/
		printf(" MESSAGE SENT TO SERVER : %s", input);          /* actual message sent over to the server*/

	}

	else if ( strstr(input, "3") !=NULL) {                 /* this if loop is used satisfy the withdraw operation of the program*/
		printf("please enter the amount to withdraw : ");
		scanf ("%s", amount);
		for (i=0; input[i] != '\0'; ++i);                         /*for loop is used to concatenate the amount field to the end of transaction type and account type */
		for (j=0; amount[j]!= '\0'; ++j, ++i)
	{
		input[i]=amount[j];
	}
		input[i]='\0';											/*places a null character at the end of the string*/
		msg_len=strlen(input)+1;
		printf(" size of message %d", msg_len);
		printf(" MESSAGE SENT TO SERVER : %s", input);

	}

	else if ( strstr(input, "4") !=NULL) {                                  /* this if loop is used satisfy the transfer operation of the program*/
		printf("please enter the amount to transfer : ");
		scanf ("%s", amount);
		for (i=0; input[i] != '\0'; ++i);                          /*this loop is used to concatenate the amount field to the end of transaction type and account type */
		for (j=0; amount[j]!= '\0'; ++j, ++i)
	{
		input[i]=amount[j];
	}
		input[i]='\0';											/*places a null character at the end of the string*/		
		msg_len=strlen(input)+1;
		printf(" size of message %d", msg_len);
		printf(" MESSAGE SENT TO SERVER : %s", input);

	}  
	else if ( strstr(input, "5") !=NULL) {                /* this if loop is used satisfy the terminate operation of the program*/
		for (i=0; input[i] != '\0'; ++i);
		input[i]='\0';
		msg_len=strlen(input)+1;
		printf(" size of message %d", msg_len);            
		printf(" MESSAGE SENT TO SERVER : %s", input);
		bytes_sent = send(sock_client, input, msg_len, 0);                /* sents message over to the server */

	   /* get response from server */
	  
	   bytes_recd = recv(sock_client, modifiedSentence, STRING_SIZE, 0); 
	   int message_length = strlen(modifiedSentence)+1;

	   printf("\nThe response from server is:\n");
	   printf("%s\n\n", modifiedSentence);
	   printf("\n size of received message : %d", message_length);
	   break;
	}
	
   bytes_sent = send(sock_client, input, msg_len, 0);          /* sents message over to the server */

   /* get response from server */
  
   bytes_recd = recv(sock_client, modifiedSentence, STRING_SIZE, 0); 
   int message_length = strlen(modifiedSentence)+1;

   printf("\nThe response from server is:\n");
   printf("%s\n\n", modifiedSentence);
   printf("\n size of received message : %d", message_length);
   }

   /* close the socket */

   close (sock_client);
}
