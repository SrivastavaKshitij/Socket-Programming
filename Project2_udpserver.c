/* udp_server.c */
/* Programmed by Kshitij Srivastava*/
/* November, 2015 */

#include <ctype.h>          /* for toupper */
#include <stdio.h>          /* for standard I/O functions */
#include <stdlib.h>         /* for exit */
#include <string.h>         /* for memset */
#include <sys/socket.h>     /* for socket, sendto, and recvfrom */
#include <netinet/in.h>     /* for sockaddr_in */
#include <unistd.h>         /* for close */

#define STRING_SIZE 1024

/* SERV_UDP_PORT is the port number on which the server listens for
   incoming messages from clients. You should change this to a different
   number to prevent conflicts with others in the class. */

#define SERV_UDP_PORT 9898
/* creates a packet structure to store correctly received packet*/
struct data_packet{
    unsigned short seq_no;
    char data[80];
    unsigned short size;
    
} pkt;
int simulate(double loss_rate, int packet_delay);
int ACKsimulate (double ack_loss_rate);
int main(void) {

   int sock_server;  /* Socket on which server listens to clients */

   struct sockaddr_in server_addr;  /* Internet address structure that
                                        stores server address */
   unsigned short server_port;  /* Port number used by server (local port) */

   struct sockaddr_in client_addr;  /* Internet address structure that
                                        stores client address */
   unsigned int client_addr_len;  /* Length of client address structure */

   char sentence[STRING_SIZE];  /* receive message */
   char modifiedSentence[STRING_SIZE]; /* send message */
   unsigned int msg_len;  /* length of message */
   int bytes_sent, bytes_recd, pos, cntr; /* number of bytes sent or received */
   unsigned int i;  /* temporary loop variable */
   unsigned short seq_num,size_data;
   int sim,acked;
   char data[80];
   unsigned short ack1;
   FILE *fp = (FILE*)fopen("out.txt","w");
   int success_packet=0;  									/* no of data packets received successfully*/
   int count=0; 											/* Total number of data bytes received which are delivered to user*/
   int duplicate_packets=0; 								/* Total number of duplicate data packets received */
   int dropped_packets=0; 									/* Number of data packets received but dropped due to loss*/ 
   int total_packets=0; 									/* Total number of data packets received including those that were successful, those lost, and duplicates*/ 
   int acks_sent=0; 										/* Number of ACKs transmitted without loss*/
   int acks_lost = 0; 										/*Number of ACKs generated but dropped due to loss*/
   int acks_total=0; 										/* Total number of ACKs generated (with and without loss)*/
   /* open a socket */

   if ((sock_server = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
      perror("Server: can't open datagram socket\n");
      exit(1);
   }

   /* initialize server address information */

   memset(&server_addr, 0, sizeof(server_addr));
   server_addr.sin_family = AF_INET;
   server_addr.sin_addr.s_addr = htonl (INADDR_ANY);  /* This allows choice of
                                        any host interface, if more than one
                                        are present */
   server_port = SERV_UDP_PORT; /* Server will listen on this port */
   server_addr.sin_port = htons(server_port);

   /* bind the socket to the local server port */

   if (bind(sock_server, (struct sockaddr *) &server_addr,
                                    sizeof (server_addr)) < 0) {
      perror("Server: can't bind to local address\n");
      close(sock_server);
      exit(1);
   }

   /* wait for incoming messages in an indefinite loop */

   printf("Waiting for incoming messages on port %hu\n\n", 
                           server_port);

   client_addr_len = sizeof (client_addr);
   
    int ack2;												/* use to store the ack for last correctly received packet*/
    int expected_seqno=0;									/* intialises expected sequence number to 0*/
    int w, pkt_delay;										/* window size and packet delay*/
	float pkt_loss_rate,ack_loss_rate;						/* packet loss rate and ack loss rate*/
	
	
	printf(" please specify window size between 1 to 8   :  ");
	scanf("%d", &w);
	printf(" packet loss rate : enter a real number b/w 0 and 1    :  ");
	scanf("%f", &pkt_loss_rate);
	printf(" packet delay : enter either 0 or 1    :  ");
	scanf("%d", &pkt_delay);
	printf(" ack loss rate : enter any real number b/w 0 and 1    :  ");
	scanf(" %f", &ack_loss_rate);
   for (;;) {
	   
	   bytes_recd = recvfrom(sock_server, &pkt, sizeof(pkt), 0,
                     (struct sockaddr *) &client_addr, &client_addr_len);
      
      seq_num = ntohs(pkt.seq_no);									/*  extracts sequence no from the incoming packet */
	  printf (" seq:%d  ", (seq_num%16));
	  size_data = ntohs (pkt.size);									/*  extracts count or size of data from the incoming packet */
	  printf("size:%d  \n", size_data);
	  
												/* as soon as server receives a packet with data 0, it terminates connection and prints out stats */
	  if (size_data ==0){
		  fclose(fp);
		  printf(" EOT received \n");
		  printf (" \n no of data packets received successfully : %d", success_packet);
		  printf (" \n no of bytes delivered to user : %d", count);
		  printf (" \n Total number of duplicate data packets received : %d", duplicate_packets);
		  printf (" \n no of data packets  received but dropped due to loss : %d", dropped_packets);
		  total_packets = success_packet + duplicate_packets + dropped_packets;
		  printf (" \n total number of packets received : %d", total_packets);
		  printf (" \n number of acks transmitted without loss :%d", acks_sent);
		  printf (" \n number of acks generated but dropped due to loss :%d", acks_lost);
		  acks_total = acks_lost + acks_sent;
		  printf (" \n total number of acks generated :%d ", acks_total);
		 break;
	  }
	  
	  
	  sim = simulate(pkt_loss_rate , pkt_delay);							/* calls simulate function*/
	  if (sim ==0){															/* drops packet if simulate returns 0*/	
		  printf (" Packet lost \n ");
		  dropped_packets = dropped_packets +1;
	  }
	  else if (sim==1){
																		/* if condition is true, it delivers the packet to the user*/
		if( seq_num == expected_seqno) {
			fputs(pkt.data,fp);
			printf(" packet with sequence number %hu received\n ",(seq_num%16));
			printf (" packet delivered to user with seq no : %d\n",(seq_num%16));
			expected_seqno = (expected_seqno + 1);
			/*fputs(data, fp);*/
			ack2= seq_num;
			ack1 = htons(seq_num);
			success_packet = success_packet +1;
			count = count + size_data;
			
			
			/*acked =1;*/
			acked = ACKsimulate (ack_loss_rate);							/* simulate ack function*/
			if (acked == 0){
				acks_lost = acks_lost + 1;
				printf (" ack lost \n");
			}
			else{
				/*memcpy(sentence,&htons(seq_num),sizeof(ack));*/
		    bytes_sent = sendto(sock_server, &ack1, sizeof(ack1) , 0,(struct sockaddr*) &client_addr, client_addr_len);
		 /* bytes_sent = sendto(sock_server, sentence, 2 , 0, (struct sockaddr*) &client_addr, client_addr_len);*/
		  acks_sent = acks_sent + 1;
		  printf("ack sent for packet: %hu \n",(seq_num%16));
		  
			}

		}																	/* this loop handles duplicate packets*/
		else if ( seq_num < expected_seqno) {
			printf( " duplicate packet received with sequence number %hu \n", (seq_num%16));
			duplicate_packets = duplicate_packets + 1;
			acked = ACKsimulate (ack_loss_rate);							/* simulate ack function*/
			if (acked == 0){
				acks_lost = acks_lost + 1;
				printf (" ack lost");
			}
			else{
			
		  bytes_sent = sendto(sock_server, &ack1, sizeof(ack1) , 0, (struct sockaddr*) &client_addr, client_addr_len);
		  acks_sent = acks_sent + 1;
		  printf("ack sent for packet: %hu\n",(ack2%16));
		  
			}
			
			
			
		}																	/* this loop handles out of order packets*/
		else if ( seq_num > expected_seqno) {
			printf (" out of order packet received with sequence number %hu \n", (seq_num%16));
			duplicate_packets = duplicate_packets + 1;
			acked = ACKsimulate (ack_loss_rate);								/* simulate ack function*/
			if (acked == 0){
				acks_lost = acks_lost + 1;
				printf (" ack lost");
			}
			else{
				if (expected_seqno == 0){
					printf (" do nothing");
				}
				else{
			/*memcpy(sentence,&ack1,sizeof(ack1));	*/
		  bytes_sent = sendto(sock_server, &ack1, sizeof(ack1) , 0, (struct sockaddr*) &client_addr, client_addr_len);
		  acks_sent = acks_sent + 1;
		  printf("ack sent for packet: %hu \n",(ack2%16));
				}
			}
		}	
	    
		 
	      
	    
	  }
	  }	  
      }
   
  


/* This function is used to simulate packet loss and packet delay 
returns 0 if packet lost else 1
adds random delay to the packet if packetDelay is 1 */

int simulate(double loss_rate, int packet_delay){
    
    double succs_prob, delay,counter;
	int flag;
    /* generate a randum number for success probability */
    succs_prob = (double)rand();
    succs_prob = succs_prob/(double)RAND_MAX;
    //printf("success probability %lf, %lf \n",succs_prob,loss_rate);
    /* if success probability is less than loss rate then return 0 */
   if (succs_prob < loss_rate) {
      /*printf("packet lost \n");*/
	flag= 0;
    }
	else{
    if(packet_delay==1){
	/* add random delay */
      delay = ((double)rand())*1000000/(double)RAND_MAX;
	for(counter =1; counter < delay; counter += 1){}
	printf("packet delayed \n");
	flag= 1;
    }
    else{
    /* packet received successfully */
    flag= 1;  
	}  
}
return flag;
}

/* This function simulates the loss of ACK packets
   return 0 if ACK is lost else 1*/

   
int ACKsimulate (double ack_loss_rate){
  
    /* generate ACK success probability */
    double ack_succs_prob = ((double)rand()/(double)RAND_MAX);
    
    //printf("ACK loss prob %lf\n",ack_succs_prob);
    /* if success probability is less than loss rate */
    if ( ack_succs_prob < ack_loss_rate){
      
	/*printf("ack lost\n");*/
	return 0;
      }
    else{
    /* ACK sent successfully */
    return 1;
} 
}

