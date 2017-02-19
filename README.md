# Socket-Programming
Contains source code for two socket programming projects using C language.

#PROJECT 1 => Task is to design and implement a server which manages a bank account and a client that interacts with the server using TCP to perform transactions on the account. 

SERVER SIDE -->
Each server maintains one checking account and one savings account both of which will be accessed by a single client. When the server starts, the balance in the accounts is initialized to 0. The client can ask the server to perform any of the following transactions:
• check balance in an account
• deposit a specified amount to an account
• withdraw a specified amount from an account
• transfer a specified amount from one account to the other

In the client-server transactions, certain assumptions were taken into account such as transactions were done in whole dollar amount (no cents) and that the transactions never exceeded a million dollar amount. Amount was withdrawn in the multiples of 20 and amount withdrawn could not be greater than the checking balance. Client had the option of requesting withdrawal from any bank account but server should permit withdrawal from checking account only. However, transfer of money from checking to saving and vice-versa was allowed. All the above mentioned constrained were to be checked on the server side and if any of the conditions were violated then server should stop the transaction and display an error.


The message from the client to the server will specify the type of transaction requested, the amount (if applicable), and the account on which the transaction is requested. The server’s response contains the type of transaction, an error code if any, the account (savings or checking), the balance before the transaction, and the balance after the transaction (in case of a deposit or withdrawal that was successful). For a transfer, the response should only give information about the account into which the transfer took place. The server must display on its standard output the details of the message received from the client and the response returned. This should include the size (number of bytes) of the message and response.

CLIENT SIDE -->
The client always waits for a response to a transaction request before sending the next request. After the client connects to the server, it may send many transaction requests over the same connection before disconnecting. It may then connect again to send more requests.
The client should prompt the user to enter a transaction type, and the account and amount (if applicable). The client displays on its standard output both the message sent and the response from the server. This should include the size (number of bytes) of the message and response.



#PROJECT 2 ==> Task is to implement a Transport Layer protocol to transmit data with Reliable Data Transfer from a client (Sender) to a server (Receiver) in the presence of channel errors and loss. The protocol to be implemented by you is the Go-Back-N protocol. 

The protocol will be unidirectional in which data is sent in one direction only (client to server) with acknowledgements being sent in the reverse direction (server to client). Only positive ACKs are used. The transmission of packets will be done over UDP (that represents an unreliable network layer channel) using fixed UDP ports.

Packet Formats
The format of a data packet is shown in the figure below. Each data packet contains a 4-byte long header followed by a number of data characters. The header contains 2 fields, each of length 16 bits (2 bytes) as shown in the figure. You must convert the values in these fields into the network byte order when they are transmitted, and convert them back to host byte order when they are received.
The first field of the header is a count of the number of data characters in the packet. This value must be in the range 0 through 80. If the count is 0, then there are no data characters in the packet.

<----------------- 16 bits (2 bytes) ------------------> 
--------------------------------------------------------
            | Count (no. of data bytes) | 
-------------------------------------------------------- 
            | Packet Sequence Number | 
-------------------------------------------------------- 
                | Data Bytes | ... | 
--------------------------------------------------------

The second field of the header is the packet sequence number. Each packet transmitted by the client is assigned a sequence number in the range 0 through 15.
The format of an acknowledgement packet that is returned from the server to the client is shown in the figure below:

<----------------- 16 bits (2 bytes) ------------------> 
-------------------------------------------------------- 
                | ACK Sequence Number | 
--------------------------------------------------------

Acknowledgements are transmitted (from the server to the client) as separate packets of fixed length, 2 bytes. The first and only field of the ACK packet is the ACK sequence number, which may be in the range 0 through 15. This is the sequence number of the data packet being acknowledged by this ACK.


                                                            General Actions
The server starts by printing out its port number. It then prompts the user to enter some configuration parameters (see later section for details). It then waits for data packets to arrive from the client.
The client starts by prompting the user to enter the hostname and port number for the server. It then prompts the user to enter the name of the file to be transferred. Next the client prompts the user to enter some configuration parameters (see later section for details).
The client then reads the input file and sends it to the server in a series of packets as described below. The server receives the file and stores it with the name out.txt. When the file transfer is complete, both the client and the server terminate execution.
The client constructs packets by reading lines one at a time from the input file. Each line in the input file contains a sequence of printable characters (no control characters, etc.), with no more than 80 characters on a line. The “newline” character read from the file at the end of each line is also transmitted in the packet and is included within the limit of 80 characters per line. You should note that the “newline” character is not the same as the “null” character used by C as a string terminator. You should not include any “null” characters in the line to be transmitted in a packet.
The client transmits each line to the server in a separate packet. The server receives the packets and puts their data into distinct lines in the output file. You must make sure that duplicate packets are discarded and do not get their data stored in the output file.
When the client is finished transmitting all the lines in the data file, and has received ACKs for all of them, it will send a special last packet called EOT signifying “End of Transmission”. This packet will have a Count of 0 and no data characters. It will have a Sequence Number that is the next sequence number that would have been used if this were a valid data packet. It is important that this packet be transmitted only after the client has received ACKs for all transmitted data packets. The server will not transmit an ACK for the EOT packet, and the client will not expect any ACK to be returned for it. The client program can terminate once this packet has been transmitted. When the server receives the EOT packet, it closes the output file and also terminates.

                                                            Client Actions
Because the client must check for timeouts as well as check for incoming ACKs, it must not block on the read function call if there is no ACK in the input buffer. For this reason, the reading of an incoming packet must be done in a non-blocking fashion. Thus, if there is no incoming ACK available, the read function should not block, but should instead return without reading so you can continue in the loop to check if a timeout has occurred. Use the nonblocking version of the UDP client for your implementation.
The sequence number of successive packets transmitted by the client must range from 0 through 15 and then wrap-around to 0. This is true no matter what window size is being used. It is acceptable if you use a buffer of size 16 to store the queue of unACK’ed packets.
Timeout:
The client needs to start a timer when a data packet is transmitted. A single timer should be used for this purpose (as in the FSM given in your textbook). The semantics of this timer can be tricky, so make sure you understand exactly when the timer needs to be started, cleared, or restarted.
It is sufficient for your purpose to have a simple synchronous implementation of the timer. In this implementation, you can find the current time with a microseconds resolution using the gettime- ofday() function. Pay close attention to the structure returned by this function; both fields of this structure must be used. To start the timer, you can set a timer variable to the time at which the timer should expire (the current time plus the timeout interval). In each loop iteration, you can get the current time and compare with the time stored in the timer variable to decide whether or not the timer has expired. Be aware of the following pitfalls in using the time structures:
3
  
• You must use both the seconds and microseconds parts of the time structure; using only one of them is not acceptable.
• You should not combine the values of the seconds and microseconds parts into a single time value. Since each of the parts is a long integer, combining them together may result in an overflow and loss of information, which may cause strange behavior of the client.
• To add, subtract, or compare two time structures, always do the operation on each component (seconds or microseconds) separately, and then deal appropriately with any carry or borrow that results.



                                                            Server Actions
The server also runs in a loop; however, it can use the regular blocking version of the server pro- gram. In addition to the server actions for the Go-Back-N protocol, we will introduce one additional function: simulating loss, errors, and delay. We will need to do this because UDP clients and servers running on the same host will almost never see such behavior.
The actions of the server will be as follows:
• Start server loop.
• Wait for packet to arrive from client.
• If packet is received, check if Count field in packet is 0. If it is 0, this signifies an EOT packet, so quit loop.
• If packet is received, but Count field is not 0, this is a regular data packet. In this case, call the simulate function described below to simulate loss, errors, and delay.
• If simulate returns 0, then packet is lost or has errors; discard packet and return to start of loop.
• If simulate returns 1, then packet is correct. Process the packet according to the protocol actions of the Go-Back-N protocol. Generate an ACK if required.
• After ACK is generated, call the function ACKsimulate to simulate ACK loss.
• If the function ACKsimulate returns 0, the ACK is lost, and is not transmitted. If the function
returns 1, then transmit the ACK.
• End of loop.

                                    Functions simulate and ACKsimulate:
In the server program, the function simulate simulates loss, errors, and delays in the reception of
data packets from the client. Errors and loss are not distinguished from each other, as they both result in the packet being dropped. This function uses the configuration parameters Packet Loss Rate and Packet Delay which are read when the server starts (see below).
Below are the actions of this function:
• Generate a uniformly distributed random number between 0 and 1.
• If the random number is less than Packet Loss Rate, then the packet is dropped. Return the
value 0.
• Otherwise, the packet is not dropped. Continue to see if it should be delayed.
• If Packet Delay is 0, then packet is not delayed. Return the value 1.
• Otherwise, the packet should be delayed. In this case, generate another uniformly distributed random number between 0 and 1, and multiply it with 108 . Execute a for loop that counts up from 1 to the value that results from the above multiplication. This loop will simulate the desired delay. After the loop completes, return 1.
The function ACKsimulate simulates loss for ACKs using the configuration parameter ACK Loss Rate as follows:
• Generate a uniformly distributed random number between 0 and 1.
• If the random number is less than ACK Loss Rate, then the ACK is dropped. Return the value
0.
• Otherwise, the ACK is not dropped. Return the value 1.


                                                Client and Server Configuration Parameters
When the server starts, it prompts the user to enter values for the following configuration parameters:
• Window Size: An integer between 1 and 8.
• Packet Loss Rate: A real number between 0 and 1. • Packet Delay: An integer which is either 0 or 1.
• ACK Loss Rate: A real number between 0 and 1.
When the client starts, it prompts the user to enter values for the following configuration parameters:
• Window Size: An integer between 1 and 8. You can assume that the user will always specify the same window size for both the client and the server.
• Timeout: The user enters an integer value n in the range 1-10, and the timeout value is then stored as 10n microseconds. Note that the resultant timeout value should be stored with both seconds and microseconds components.


