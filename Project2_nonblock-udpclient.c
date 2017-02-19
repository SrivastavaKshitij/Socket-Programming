/* nonblock-udp_client.c */ 
/* Programmed by Kshitij Srivastava */
/* November, 2015 */

/* Version of udp_client.c that is nonblocking */
#include <sys/time.h>
#include <time.h>  
#include <stdio.h>          /* for standard I/O functions */
#include <stdlib.h>         /* for exit */
#include <string.h>         /* for memset, memcpy, and strlen */
#include <netdb.h>          /* for struct hostent and gethostbyname */
#include <sys/socket.h>     /* for socket, sendto, and recvfrom */
#include <netinet/in.h>     /* for sockaddr_in */
#include <unistd.h>         /* for close */
#include <fcntl.h>          /* for fcntl */
#include <math.h>

#define STRING_SIZE 1024
/* created a structure for timeout value*/
struct timeout{
	long sec;
	long usec;
} t,t1,t2;
/* created a current time structure to store the value of current time to handle timeouts*/
struct current_time{
	long sec;
	long usec;
} ct;
/* created a structure packet which contains sequence no, no of data bytes (denoted by size) and data */
struct data_packet{
    unsigned short seq_no;
    char data[80];
    unsigned short size;
    
} pkt[16];


int main(void) {

   int sock_client;  /* Socket used by client */ 

   struct sockaddr_in client_addr;  /* Internet address structure that
                                        stores client address */
   unsigned short client_port;  /* Port number used by client (local port) */

   struct sockaddr_in server_addr;  /* Internet address structure that
                                        stores server address */
   struct hostent * server_hp;      /* Structure to store server's IP
                                        address */
   char server_hostname[STRING_SIZE]; /* Server's hostname */
   unsigned short server_port;  /* Port number used by server (remote port) */

   char sentence[STRING_SIZE];  /* send message */
   char modifiedSentence[STRING_SIZE]; /* receive message */
   unsigned int msg_len;  /* length of message */
   int bytes_sent, bytes_recd,i; /* number of bytes sent or received */

   int fcntl_flags; /* flags used by the fcntl function to set socket
                       for non-blocking operation */
   FILE *fptr;
   char data[80],bsize[2],bseq[2];
   int base=0, nextseqno=0;                                  /* intialise base and next sequence no to 0 for GBN implemetation*/	
   int n ; 													/* window size*/
   unsigned short count=0, begin,seq,win=6,ack_num,nsize,nseq; /* all these variable are used in different for loops in the program*/
   unsigned short ack;
   unsigned short hnextseqno;								/* this is used to convert nextseqno into htons nextseqno*/
   unsigned short length;									/* used to measure length of data received from fgets function*/
   unsigned short hlength;									/* converts length into network byte host number*/
   int a,z;
   int tout;
   int no_packets=0; 											/* no of data packets transmitted*/
   int sum_databytes=0; 										/* total number of data bytes transmitted*/
   int retrans=0; 												/* no of retransmissions*/
   int total_packets; 											/* total no of data packets transmitted*/
   int ack_recd=0; 												/* no of acks received*/
   int no_timeouts=0; 											/* no of times timeout expired*/
   float pow1;
   int y;
   int x=1;
   
  
	   
   /* open a socket */

   if ((sock_client = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
      perror("Client: can't open datagram socket\n");
      exit(1);
   }

   /* Note: there is no need to initialize local client address information
            unless you want to specify a specific local port.
            The local address initialization and binding is done automatically
            when the sendto function is called later, if the socket has not
            already been bound. 
            The code below illustrates how to initialize and bind to a
            specific local port, if that is desired. */

   /* initialize client address information */

   client_port = 0;   /* This allows choice of any available local port */

   /* Uncomment the lines below if you want to specify a particular 
             local port: */
   /*
   printf("Enter port number for client: ");
   scanf("%hu", &client_port);
   */

   /* clear client address structure and initialize with client address */
   memset(&client_addr, 0, sizeof(client_addr));
   client_addr.sin_family = AF_INET;
   client_addr.sin_addr.s_addr = htonl(INADDR_ANY); /* This allows choice of
                                        any host interface, if more than one 
                                        are present */
   client_addr.sin_port = htons(client_port);

   /* bind the socket to the local client port */

   if (bind(sock_client, (struct sockaddr *) &client_addr,
                                    sizeof (client_addr)) < 0) {
      perror("Client: can't bind to local address\n");
      close(sock_client);
      exit(1);
   }

   /* make socket non-blocking */
   fcntl_flags = fcntl(sock_client, F_GETFL, 0);
   fcntl(sock_client, F_SETFL, fcntl_flags | O_NONBLOCK);

   /* end of local address initialization and binding */

   /* initialize server address information */
	
   printf("Enter hostname of server: ");
   scanf("%s", server_hostname);
   if ((server_hp = gethostbyname(server_hostname)) == NULL) {
      perror("Client: invalid server hostname\n");
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

   /* user interface */
   struct timeval tv;
   
	printf (" enter window size : ");
	scanf( "%d", &n);
	
	printf(" enter value for timeout ( range: 1-10) : ");
	scanf(" %d",&a);

	/*t.usec = (10^a) % (10^6);
	t.sec = ((10^a) - t.usec)/10^6;*/
	
	for (y=0; y<a;y++){
		x = x*10;
	}
	t.sec = x / 1000000;									    /* converting timeout value into seconds and microseconds*/
	t.usec = x % 1000000;
	printf(" t.sec :%ld", t.sec);
	printf("t.usec : %ld  ", t.usec);
	
   printf("Please enter a filename:\n");
   scanf("%s", sentence);
   msg_len = strlen(sentence) + 1;
   fptr = fopen(sentence,"r");
   if(fptr == NULL){
     printf("Cannot open file: %s\n",sentence);
     exit(1);
	 }
	 
	FILE *fp;													/* this loop counts the total no of lines in the input file*/
	fp = fopen(sentence,"r");
	
	int lines=0;
	char line[80];
	while( fgets(line,sizeof(line),fp) != NULL){
		lines++;
	}
	
	fclose(fp);
	printf (" no of lines : %d", lines);
	 
   while (1){
	   
	   while  (nextseqno< base + n){
		    if (fgets(data,80,fptr)!=NULL){						/* reads data from input file */
		
		hnextseqno = htons(nextseqno);							/* converts sequence no into network host byte*/
		pkt[nextseqno%16].seq_no=hnextseqno;					/* puts network host byte into packet*/
		sprintf(pkt[nextseqno%16].data,"%s",data);				/* puts data into packet*/
		length = strlen(pkt[nextseqno%16].data);
		hlength = htons(length);
		pkt[nextseqno%16].size= hlength;						/* puts count value in the packet*/
		
		bytes_sent = sendto(sock_client, &pkt[nextseqno%16], sizeof(pkt[nextseqno%16]), 0,
            (struct sockaddr *) &server_addr, sizeof (server_addr));
		printf("\n packet no sent : %d with bytes %d",(nextseqno%16), length);
		no_packets=no_packets+1;
		sum_databytes=sum_databytes+length;
		if (nextseqno==base){
					gettimeofday(&tv,NULL);						/* calculates the time at which the packet was sent */
																/* takes care of overflow situation*/
					if ((tv.tv_usec + t.usec) >= 1000000){
						t1.sec = tv.tv_sec + t.sec + ( tv.tv_usec + t.usec)/1000000;
						t1.usec = ( tv.tv_usec + t.usec) % 1000000;
					}
					else{
						t1.sec = tv.tv_sec + t.sec;
						t1.usec=tv.tv_usec + t.usec;
					}
					printf(" t1.sec :%ld , t1.usec : %ld", t1.sec, t1.usec );
					
					
					
							}
		nextseqno = (nextseqno+1);
		/*printf( " next expected seq no: %d   ",(nextseqno%16));	*/	
		/*printf(" seq : %d , base : %d, n : %d", nextseqno, base, n);*/
	   }
	   else{
		   if (n==1){			/* this loop takes care of situation when window size= 1, and the program has reached the end of input file*/
			printf(" EOT Send");
			no_packets=no_packets+1;
			
			hnextseqno = htons(nextseqno);
			pkt[nextseqno%16].seq_no=hnextseqno; 
			length =0;
			hlength = htons(length);
			pkt[nextseqno%16].size= hlength;
			bytes_sent = sendto(sock_client, &pkt[nextseqno%16], sizeof(pkt[nextseqno%16]), 0,
            (struct sockaddr *) &server_addr, sizeof (server_addr)); 
			printf("\n packet no sent : %d with bytes %d",(nextseqno%16), length);
			printf(" \n original number of packets send : %d", no_packets);
			printf(" \n total number of data bytes send : %d", sum_databytes);
			printf (" \n  total number of retransmission : %d", retrans);
			total_packets = no_packets + retrans;
			printf (" \n total number of packets sent :%d", total_packets);
			printf (" total number of acks received : %d", ack_recd);
			printf (" \n total number of timeouts : %d", no_timeouts);
			
			break;
			}
			else{
		
			length=0;
			break;
			}
	   }
	   }
   
  
   printf("Waiting for response from server...\n");
   
   do {  /* loop required because socket is nonblocking */
		gettimeofday(&tv,NULL);
		
		/*if ((tv.tv_usec - t1.usec) <0){
			tv.tv_sec = tv.tv_sec -1;
			tv.tv_usec = tv.tv_usec + 1000000;
		}
		ct.sec = tv.tv_sec;
		ct.usec = tv.tv_usec;
		
		if( ct.sec >= t1.sec && ct.usec >= t1.usec){
			tout=1;
			bytes_recd=1;
			break;
		}
		else{
			tout=0;
			bytes_recd = recvfrom(sock_client, &ack, sizeof(ack), 0,
		(struct sockaddr *) 0, (int *) 0);
		}*/
		ct.sec = tv.tv_sec;													/* this function takes care of timeout situation*/
		ct.usec = tv.tv_usec;
		
		if ( ct.sec < t1.sec || ((ct.sec == t1.sec ) && (ct.usec < t1.usec)))
		{
		tout=0;				
		bytes_recd = recvfrom(sock_client, &ack, sizeof(ack), 0,
		(struct sockaddr *) 0, (int *) 0);
		
		}
   
		else{
			
			tout=1;
			bytes_recd=1;
			break;
			
		}
   }
   
   while (bytes_recd <=0);
	
	
	if (tout==0){							/* this if function takes care of the situation when ack has been received successfully*/ 
	printf("response recd\n");
	ack_num = ntohs (ack);
	printf(" ack number received is : %d\n", (ack_num%16));
    ack_recd = ack_recd +1;
											/* acks , base number and nextseqno are modified according to GBN implementation*/
	
	if (ack_num == base){
		base =  base +1;
		}	
	else if(ack_num < base){
		/*base = base + ack_num;*/
		printf(" ignore ack");
		}
	
	else if (ack_num > base){
	base = base + (ack_num-base+1);
		}
	if (/*base == nextseqno &&*/ ack_num==lines-1){
		printf(" EOT Send");
		hnextseqno = htons(nextseqno);
		pkt[nextseqno%16].seq_no=hnextseqno; 
		length =0;
		hlength = htons(length);
		pkt[nextseqno%16].size= hlength;
		bytes_sent = sendto(sock_client, &pkt[nextseqno%16], sizeof(pkt[nextseqno%16]), 0,
            (struct sockaddr *) &server_addr, sizeof (server_addr)); 
			printf("\n packet no sent : %d with bytes %d \n",(nextseqno%16), length);
			printf(" \n original number of packets send : %d", no_packets);
			printf(" \n total number of data bytes send : %d", sum_databytes);
			printf (" \n  total number of retransmission : %d", retrans);
			total_packets = no_packets + retrans;
			printf (" \n total number of packets sent :%d", total_packets);
			printf (" \n total number of acks received : %d", ack_recd);
			printf (" \n total number of timeouts : %d", no_timeouts);
		break;
	}
	}
	
	if (tout==1){							/*Retransmission - this function takes care of the situation when timeout has occured*/
		no_timeouts = no_timeouts+1;
		printf (" time out expired for packet no : %d", (base%16));
		printf("\n packet no sent : %d with bytes: %d    ",(base%16), strlen(pkt[base%16].data) );
		bytes_sent = sendto(sock_client, &pkt[base%16], sizeof(pkt[base%16]), 0,
            (struct sockaddr *) &server_addr, sizeof (server_addr));
		
		
		retrans = retrans +1;
		gettimeofday(&tv,NULL);
					if ((tv.tv_usec + t.usec) >= 1000000){
						t1.sec = tv.tv_sec + t.sec + ( tv.tv_usec + t.usec)/1000000;
						t1.usec = ( tv.tv_usec + t.usec) % 1000000;
					}
					else{
						t1.sec = tv.tv_sec + t.sec;
						t1.usec=tv.tv_usec + t.usec;
					}
					printf(" t1.sec :%ld , t1.usec : %ld", t1.sec, t1.usec );
		
		for (z=base+1; z<=nextseqno-1; z=z+1){
			bytes_sent = sendto(sock_client, &pkt[z%16], sizeof(pkt[z%16]), 0,
            (struct sockaddr *) &server_addr, sizeof (server_addr)); 
			printf("\n packet no sent : %d with bytes %d   ",(z%16), strlen(pkt[z%16].data));
			
			
			retrans = retrans +1;
		}
			
	}
	}
   

     /* close the socket */

   close (sock_client);
}



