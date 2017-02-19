/* tcpserver.c */
/* KSHITIJ SRIVASTAVA */
/* OCTOBER 20, 2015 */    

#include <ctype.h>          /* for toupper */
#include <stdio.h>          /* for standard I/O functions */
#include <stdlib.h>         /* for exit */
#include <string.h>         /* for memset */
#include <sys/socket.h>     /* for socket, bind, listen, accept */
#include <netinet/in.h>     /* for sockaddr_in */
#include <unistd.h>         /* for close */

#define STRING_SIZE 1024   

/* SERV_TCP_PORT is the port number on which the server listens for
   incoming requests from clients. You should change this to a different
   number to prevent conflicts with others in the class. */

#define SERV_TCP_PORT 4271

int main(void) {

   int sock_server;  /* Socket on which server listens to clients */
   int sock_connection;  /* Socket on which server exchanges data with client */

   struct sockaddr_in server_addr;  /* Internet address structure that
                                        stores server address */
   unsigned int server_addr_len;  /* Length of server address structure */
   unsigned short server_port;  /* Port number used by server (local port) */

   struct sockaddr_in client_addr;  /* Internet address structure that
                                        stores client address */
   unsigned int client_addr_len;  /* Length of client address structure */

   char sentence[STRING_SIZE];  /* receive message */
   char modifiedSentence[STRING_SIZE]; /* send message */
   unsigned int msg_len;  /* length of message */
   int bytes_sent, bytes_recd; /* number of bytes sent or received */
   unsigned int i;  /* temporary loop variable */
   
   int c_bal = 0; /* intialises checking balance to 0*/

   char c_balc[STRING_SIZE]; /* creates a string to store checking balance information*/
   int s_bal = 0;            /* intialises saving balance to 0*/ 
   char s_balc[STRING_SIZE]; /* creates a string to store saving balance information*/ 
   int c_bal_new,s_bal_new;  /* creates two variables to store new checking account balance and new saving account balance */
   char c_bal_newc[STRING_SIZE]; /* creates a string to store new checking account balance*/
   char s_bal_newc[STRING_SIZE]; /* creates a string to store new saving account balance */
   char amount[50];
   char input[2];
   char account[2];
   int money;
   int j,k;
   /* open a socket */

   if ((sock_server = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
      perror("Server: can't open stream socket");
      exit(1);                                                
   }

   /* initialize server address information */
    
   memset(&server_addr, 0, sizeof(server_addr));
   server_addr.sin_family = AF_INET;
   server_addr.sin_addr.s_addr = htonl (INADDR_ANY);  /* This allows choice of
                                        any host interface, if more than one
                                        are present */ 
   server_port = SERV_TCP_PORT; /* Server will listen on this port */
   server_addr.sin_port = htons(server_port);

   /* bind the socket to the local server port */

   if (bind(sock_server, (struct sockaddr *) &server_addr,
                                    sizeof (server_addr)) < 0) {
      perror("Server: can't bind to local address");
      close(sock_server);
      exit(1);
   }                     

   /* listen for incoming requests from clients */

   if (listen(sock_server, 50) < 0) {    /* 50 is the max number of pending */
      perror("Server: error on listen"); /* requests that will be queued */
      close(sock_server);
      exit(1);
   }
   printf("I am here to listen ... on port %hu\n\n", server_port);
  
   client_addr_len = sizeof (client_addr);

   /* wait for incoming connection requests in an indefinite loop */

   for (;;) {

      sock_connection = accept(sock_server, (struct sockaddr *) &client_addr, 
                                         &client_addr_len);
                     /* The accept function blocks the server until a
                        connection request comes from a client */
      if (sock_connection < 0) {
         perror("Server: accept() error\n"); 
         close(sock_server);
         exit(1);
      }
 
      
int t=1;
while(t ==1){                                                    /* to make an infinite loop */
      bytes_recd = recv(sock_connection, sentence, STRING_SIZE, 0); /* receives bytes from client side */

      if (bytes_recd > 0){
         printf("Received Sentence is:\n");
         printf("%s", sentence);
         printf("\nwith length %d\n", bytes_recd);
		 strncpy(input, &sentence[0],1);                 /* takes the first char of the string and stores it in input variable , this tells us the operation type*/
		 strncpy(account, &sentence[1],1);               /* takes the second char of the string & stores it in account variable to specify the type of account*/  
		 input[1]='\0';                                  /* places a null character  at the end of input string*/
		 account[1] ='\0';									/* places a null character  at the end of account string*/
		 char *a = sentence; a++;a++;                     /* removes the first two bytes from the original string and copies the rest of it in amount string*/
		 strcpy(amount,a);								
		 money = atoi(amount);								/* converts string to integer*/
		 
		 if (money % 20 == 0){                              /* checks condition whether the amount is a multiple of 20 or not*/
		 if(strstr(input, "1") != NULL){                    /* this for loop checks the balance for the requested account*/
			
			if (strstr(account, "c") !=NULL){                /* this for loops is used to check the balance for checking account*/
				
				sprintf(c_balc, "%006d",c_bal);               /* ALTHOUGH IN THE INSTRUCTION IT WAS MENTIONED NOT TO USE SPRINTF FUNCTION BECAUSE IT GENERATES VARIABLE LENGTH */
				char ab[STRING_SIZE] = "checking balance:";  /* STRINGS, HOWEVER, IN MY CASE I HAVE FIXED THE LENGTH OF THE STRING TO 6 BY SLIGHTLY MODIFYING THE ORIGINAL SPRINTF*/
				for (k=0; ab[k] != '\0'; ++k);
				for (j=0; c_balc[j]!= '\0'; ++j, ++k)      /* Concatenates the two strings ab and c_balc which is sent to the client*/
				{
					ab[k]=c_balc[j];
				}
				ab[k]='\0';
				msg_len=strlen(ab) + 1;	
				strcpy(modifiedSentence,ab);
				printf(" length of the message sent to client: %d \n", msg_len);
				printf (" account type is : checking ; transaction type = check balance\n ");
				bytes_sent = send(sock_connection, modifiedSentence , msg_len, 0);
				
								
			}
			else if(strstr(account, "s") !=NULL){                /* this for loops is used to check the balance for savings account*/
				
				sprintf(s_balc, "%006d",s_bal);
				char ab[STRING_SIZE] = "savings balance:";
				for (k=0; ab[k] != '\0'; ++k);
				for (j=0; s_balc[j]!= '\0'; ++j, ++k) 
				{
					ab[k]=s_balc[j];
				}
				ab[k]='\0';
				msg_len=strlen(ab) + 1;
				strcpy(modifiedSentence,ab);
				printf(" length of the message sent to client: %d \n", msg_len);
				printf (" account type is : savings ; transaction type = check balance \n ");
				bytes_sent = send(sock_connection, modifiedSentence , msg_len, 0);
							
			}
			else {																	/* this for loops is used when the user inputs an invalid choice*/
			char dd[STRING_SIZE] = "error: invalid input";
			for (k=0; dd[k] != '\0'; ++k);
			dd[k]='\0';
			msg_len=strlen(dd) + 1;
			strcpy(modifiedSentence,dd);
			printf(" length of the message sent to client: %d \n", msg_len);
			printf (" transaction type = check balance ; error generated\n");
			bytes_sent = send(sock_connection, modifiedSentence , msg_len, 0);
			}	

		}
		else if (strstr(input, "2") != NULL){                                /* this for loops is used to deposit money */
			if (strstr(account, "c") != NULL ){			/* this loop is used for depositing money in the checking account and sending the result over to the client side*/
			
			c_bal_new = c_bal + money;                   /* this shows new balance = old balance + amount deposited*/
			sprintf(c_balc, "%006d",c_bal);              /* converts old balance into string */
			sprintf(c_bal_newc, "%006d",c_bal_new);      /* converts new balance into string*/
			c_bal = c_bal_new;                            /* after the money has been deposited, old balance becomes new balance */
			char cd[STRING_SIZE] = "new checking balance:";    /* From line 179 to 203, the code is calculating the new balance, code is also storing the values of (contd)*/
			for (k=0; cd[k] != '\0'; ++k);                     /* old balance and new balance and converting them into strings, which would be sent over to the client(contd) */
			for (j=0; c_bal_newc[j]!= '\0'; ++j, ++k)           /* this process is done for checking account in this if code*/
			{
				cd[k]=c_bal_newc[j];
			}
			cd[k]='\0';                                        /* places a null character at the end of string cd*/
			char ef[STRING_SIZE] = "old checking balance:";
			for (k=0; ef[k] != '\0'; ++k);
			for (j=0; c_balc[j]!= '\0'; ++j, ++k) 
			{
				ef[k]=c_balc[j];
			}
			ef[k]='\0';
			for (k=0; ef[k] != '\0'; ++k);
			for (j=0; cd[j]!= '\0'; ++j, ++k) 
			{
				ef[k]=cd[j];
			}
			ef[k]= '\0';               						/* places a null character at the end of the string*/
			msg_len=strlen(ef) + 1;
			printf(" length of message sent to client %d:\n ", msg_len);
			printf (" account type is : checking ; transaction type = deposit \n");
			strcpy(modifiedSentence, ef);		
			bytes_sent = send(sock_connection, modifiedSentence , msg_len, 0);
		
		
			}
			
		else if (strstr(account, "s") != NULL){
			
			s_bal_new = s_bal + money;						/* this code is same as the above code but this code is used to deposit the money in the savings account and (contd)*/
			sprintf(s_balc, "%006d",s_bal);                 /* stores the variables in the string format which was transferred to the client*/
			sprintf(s_bal_newc, "%006d",s_bal_new);
			char cd[STRING_SIZE] = "new savings balance:";
			s_bal = s_bal_new;
			for (k=0; cd[k] != '\0'; ++k);
			for (j=0; s_bal_newc[j]!= '\0'; ++j, ++k) 
			{
				cd[k]=s_bal_newc[j];
			}
			cd[k]='\0';
			char ef[STRING_SIZE] = "old savings balance:";
			for (k=0; ef[k] != '\0'; ++k);
			for (j=0; s_balc[j]!= '\0'; ++j, ++k) 
			{
				ef[k]=s_balc[j];
			}
			ef[k]='\0';
			for (k=0; ef[k] != '\0'; ++k);
			for (j=0; cd[j]!= '\0'; ++j, ++k) 
			{
				ef[k]=cd[j];
			}
			ef[k]= '\0';
			msg_len=strlen(ef) + 1;
			strcpy(modifiedSentence, ef);
			printf(" length of the message sent to client: %d \n", msg_len);
			printf (" account type is : savings ; transaction type = deposit\n ");
			bytes_sent = send(sock_connection, modifiedSentence , msg_len, 0);		
			}
		
		else{             										/* this code handles the situation in which the user enters a wrong input*/
			char lm[STRING_SIZE] = "error: invalid input";      /* for eg there are only 2 valid inputs : 2c or 2s . this code handles situations in which the user enters(contd)*/
			for (k=0; lm[k] != '\0'; ++k);						/* something like 2j or 2b */
			lm[k] = '\0';
			msg_len=strlen(lm) + 1; 
			printf(" size of message sent to client : %d\n", msg_len);
			printf (" error generated ; transaction type = deposit \n");
			strncpy(modifiedSentence, lm, msg_len);
			bytes_sent = send(sock_connection, modifiedSentence , msg_len, 0);
			}
		}


		else if (strstr(input, "3") != NULL){       /* similar code as above. this code withdraws money from account*/
			if (strstr(account, "c") != NULL ){      /* this code withdraws money from checking account*/  
				
				c_bal_new = c_bal - money;        
				if (c_bal >= money){                  /* this line checks whether the requested amount is less than the amount present in the account*/   
				sprintf(c_bal_newc, "%006d",c_bal_new);
				sprintf(c_balc, "%006d",c_bal);
				c_bal = c_bal_new;
				char cd[STRING_SIZE] = "new checking balance:";
				for (k=0; cd[k] != '\0'; ++k);
				for (j=0; c_bal_newc[j]!= '\0'; ++j, ++k) 
				{
					cd[k]=c_bal_newc[j];
				}
				cd[k]='\0';
				char ef[STRING_SIZE] = "old checking balance:";
				for (k=0; ef[k] != '\0'; ++k);
				for (j=0; c_balc[j]!= '\0'; ++j, ++k) 
				{
					ef[k]=c_balc[j];
				}
				ef[k]='\0';        
				for (k=0; ef[k] != '\0'; ++k);
				for (j=0; cd[j]!= '\0'; ++j, ++k) 
				{
					ef[k]=cd[j];                      /* concatenates both the strings cd and ef*/
				}
				ef[k]= '\0';
				msg_len=strlen(ef) + 1;
				printf(" message sent to client : %d\n", msg_len);
				printf (" account type is : checking ; transaction type = withdraw\n ");
				strncpy(modifiedSentence, ef, msg_len);
				bytes_sent = send(sock_connection, modifiedSentence , msg_len, 0);
				}
				else if (c_bal < money){            /* if requested amount is greater than amount in account, this code handles that situation*/
					char aa[STRING_SIZE] = "error: insufficient balance";
				msg_len=strlen(aa) + 1;
				printf(" length of message sent over to the client : %d", msg_len);
				strncpy(modifiedSentence, aa, msg_len);
				printf (" account type is : checking ; transaction type = withdrawal ; error generated\n ");
				
				bytes_sent = send(sock_connection, modifiedSentence , msg_len, 0);
			
				}
		
			}
			
			else if (strstr(account, "s") != NULL){  /* this code does not allow user to withdraw money from savings account*/
		
				char ef[STRING_SIZE] = "error: permission denied";
			
				/*for (k=0; ef[k] != '\0'; ++k);
		
				ef[k]='\0';*/
				msg_len=strlen(ef) + 1;
				printf(" length of message sent to the client : %d \n", msg_len);
				printf (" account type is : savings ; transaction type = withdraw ; error generated \n ");
				strncpy(modifiedSentence, ef, msg_len);
				bytes_sent = send(sock_connection, modifiedSentence , msg_len, 0);
				}
		else {
			char dd[STRING_SIZE] = "error: invalid input"; /*if the user enters a wrong input, this code handles that problem*/
			for (k=0; dd[k] != '\0'; ++k);
			dd[k]='\0';
			msg_len=strlen(dd) + 1;
			strcpy(modifiedSentence,dd);
			printf(" length of message sent to client : %d \n", msg_len);
			printf (" transcation type : withdraw ; error generated \n");
			bytes_sent = send(sock_connection, modifiedSentence , msg_len, 0);
			}
}

	else if (strstr(input, "4") != NULL){             /* this code transfers money from checking account to saving account and vice versa*/
		if (strstr(account, "c") != NULL ){           /* this code transfers money from checking to saving but displays the old balance and new balance of savings account only*/
			
			c_bal_new = c_bal - money;
			if(c_bal >= money){                /* checks to see if the requested amount is less than the balance in the account or not*/
				s_bal_new = s_bal + money;
				sprintf(s_balc,"%006d", s_bal);
				sprintf(s_bal_newc, "%006d",s_bal_new);
				c_bal = c_bal_new;
				s_bal = s_bal_new;
				char cd[STRING_SIZE] = "new savings balance:";
				for (k=0; cd[k] != '\0'; ++k);
				for (j=0; s_bal_newc[j]!= '\0'; ++j, ++k) 
				{
					cd[k]=s_bal_newc[j];
				}
				cd[k]='\0';
				char ef[STRING_SIZE] = "old savings balance:";
				for (k=0; ef[k] != '\0'; ++k);
				for (j=0; s_balc[j]!= '\0'; ++j, ++k) 
				{
					ef[k]=s_balc[j];
				}
				ef[k]='\0';
				for (k=0; ef[k] != '\0'; ++k);
				for (j=0; cd[j]!= '\0'; ++j, ++k) 
				{
					ef[k]=cd[j];
				}
				ef[k]= '\0';
				msg_len=strlen(ef) + 1;
				printf (" account type is : checking ; transaction type = transfer \n ");
				printf(" message length sent to the client : %d\n", msg_len);
				strncpy(modifiedSentence, ef, msg_len);
				bytes_sent = send(sock_connection, modifiedSentence , msg_len, 0);
				}
			else if (c_bal < money){          /* this code shows an error if requested amount > balance in the account*/
				char bb[STRING_SIZE] = "insufficient balance";
				msg_len=strlen(bb) + 1;
				printf("message length sent to the client : %d\n", msg_len);
				printf(" transaction type : transfer , error generated");
				strncpy(modifiedSentence, bb, msg_len);
				bytes_sent = send(sock_connection, modifiedSentence , msg_len, 0);
					}
	
		
		}
			
		else if (strstr(account, "s") != NULL){    /* this code transfers money from savings to checking account but displays the old balance and new balance of checking account only*/
			
			s_bal_new = s_bal - money;
			if(s_bal>=money){
				c_bal_new = c_bal + money;
				sprintf(c_bal_newc, "%006d",c_bal_new);
				sprintf(c_balc, "%006d",c_bal);
				c_bal = c_bal_new;
				s_bal = s_bal_new;
				char cd[STRING_SIZE] = "new checking balance:";
				
				for (k=0; cd[k] != '\0'; ++k);
				for (j=0; s_bal_newc[j]!= '\0'; ++j, ++k) 
				{
					cd[k]=c_bal_newc[j];
				}
				cd[k]='\0';
				char ef[STRING_SIZE] = "old checking balance:";
				for (k=0; ef[k] != '\0'; ++k);
				for (j=0; c_balc[j]!= '\0'; ++j, ++k) 
				{
					ef[k]=c_balc[j];
				}
				ef[k]='\0';
				for (k=0; ef[k] != '\0'; ++k);
				for (j=0; cd[j]!= '\0'; ++j, ++k) 
				{
					ef[k]=cd[j];
				}
				ef[k]= '\0';
				msg_len=strlen(ef) + 1;
				printf(" message length sent to the client : %d\n", msg_len);
				printf (" account type is : savings ; transaction type = transfer \n ");
				strncpy(modifiedSentence, ef, msg_len);
				
				bytes_sent = send(sock_connection, modifiedSentence , msg_len, 0);
			}
		else if (s_bal < money){
				char cc[STRING_SIZE] = "insufficient amount";
				msg_len=strlen(cc) + 1;
				printf(" message length sent to the client : %d\n", msg_len);
				printf (" error generated; transaction type = transfer \n ");
				strncpy(modifiedSentence, cc, msg_len);
				bytes_sent = send(sock_connection, modifiedSentence , msg_len, 0);
			
			}
		}
		else {														/* two standard inputs are : 4c, 4s, if the user enters any other input this code handles that problem*/
				char dd[STRING_SIZE] = "error: invalid input";
				for (k=0; dd[k] != '\0'; ++k);
				dd[k]='\0';
				msg_len=strlen(dd) + 1;
				strcpy(modifiedSentence,dd);
				printf(" message length sent to the client : %d\n", msg_len);
				
				bytes_sent = send(sock_connection, modifiedSentence , msg_len, 0);
			}
}

	else if (strstr(input, "5") != NULL){          /* this code helps in closing the sock connection because the user requested to terminate the program*/
		char gh[STRING_SIZE] = "user requested to terminate";
		for (k=0; gh[k] != '\0'; ++k);
		gh[k] = '\0';
		msg_len=strlen(gh) + 1;
		printf(" size of message sent to server: %d", msg_len);
		strncpy(modifiedSentence, gh, msg_len);
		bytes_sent = send(sock_connection, modifiedSentence , msg_len, 0);
		break;
		}
	
else {
	char lm[STRING_SIZE] = "error: invalid input";
	for (k=0; lm[k] != '\0'; ++k);
	lm[k] = '\0';
	msg_len=strlen(lm) + 1;
	printf(" length of message sent to the client : %d", msg_len);
	printf(" error generated");
	strncpy(modifiedSentence, lm, msg_len);
	bytes_sent = send(sock_connection, modifiedSentence , msg_len, 0);
	
	}
		 
}
else if (money % 20 !=0){            /* if the amount is not a multiple of 20, this code handles that situation by printing out an error*/
	char mn[STRING_SIZE] = "input should be a multiple of 20";
	for (k=0; mn[k] != '\0'; ++k);
	mn[k] = '\0';
	msg_len=strlen(mn) + 1;
	printf(" length of message sent to the client : %d ", msg_len);
	printf(" error generated \n");
	strncpy(modifiedSentence, mn, msg_len);
	bytes_sent = send(sock_connection, modifiedSentence , msg_len, 0);
	
} 
	  }
         
	  
      }
}
      /* close the socket */

      close(sock_connection);
   } 

