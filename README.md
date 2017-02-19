# Socket-Programming
Contains source code for two socket programming projects using C language.

PROJECT 1 => Task is to design and implement a server which manages a bank account and a client that interacts with the server using TCP to perform transactions on the account. 

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



PROJECT 2 ==> Task is to implement a Transport Layer protocol to transmit data with Reliable Data Transfer from a client (Sender) to a server (Receiver) in the presence of channel errors and loss. The protocol to be implemented by you is the Go-Back-N protocol. The protocol will be unidirectional in which data is sent in one direction only (client to server) with acknowledgements being sent in the reverse direction (server to client). Only positive ACKs are used. The transmission of packets will be done over UDP (that represents an unreliable network layer channel) using fixed UDP ports.

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



