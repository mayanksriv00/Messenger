************************************************ README *******************************************************************

******* INTRODUCTION *********

This is a Readme for the Messenger Application. A software to communicate with friends using Linux socket programming, thread programming and signal handling.
This project is written in C++ programming language and it used the feature of Socket, thread programming and signal handling. There are client and server cpp program in the project to run
server and client.

****** FILE details and structure *******

1. SERVER: The server is consist of two files
	1. messenger_server.h <header file for messenger_Server.cpp> 
	2. messenger_server.cpp <cpp main file ro run server>

2. CLIENT: THe client is consist of two files 
	1. messenger_client.h <header file for messenger_clinet.cpp>
	2. messenger_client.cpp <cpp file to run client>

3. MAKEFILE: That execute all commands to compile messenger_server and messenger_client and clean

4. user_info_file: THis file contains the user account instruction like: freind list, name, password
5. client_configuration_file: THis file contains the details use by the client to connect to the server <Server host and server port>
6. server_configuration_file: THis file contains the details used by server to run using port.


******* HOW TO EXECUTE *********

1. Type command < make all > : to compile and create executable of all the files together (i.e. messenger_server.cpp and messenger_server.h, messenger_client.h and , messenger_client.cpp)
2. SERVER EXECUTION: (seperate terminal)
	1. THe default port address for the server execuion is 5100, but you can change to something else if differnet port is required in file  <server_configuration_file (FILE DETAILE AND STEUCTURE point6)>
	2. Type command < ./messenger_Server user_info_file server_configuration_file> to run server using port mentioned in server_configuration_file
3. CLIENT EXECUTION:(seperate terminal)
	1. THe default port client used to connect to server is present in client_configuration_file (FILE details and structure point 5)
	2. Type command < ./messenger_client client_configuration_file <PORT NO client will use to run> >
4. Type <make clean> to clean all *.o and other executable 
NOTE: Make sure that port is free before using it.

****** HOW TO STOP *******
	1. Press CTRL+C to generate SIGINT SIGNAL to close server
	2. Type logout or exit to close client program


*******FEATURE SUPPORTED *********
This project support all the features asked 

	1. 'r' to register
	2. 'l' to login
	3. 'm freind_name message_to_send"
	4. 'ia inviter_name message_to_Send"
	5. 'i invite_freind_name message_to_send"
	6. 'exit' to exit the client program
	7. SIGINT 
	8. 'logout' to logout from the client


************ HOW TO RUN ********************
1. SERVER
	1. use seperate terminal to run server
	2. Type command <./messenger_Server user_info_file server_configuration_file> to start server.
	3. No need to do any thing in server. 
	4. Pres CTRL+C to generate SIGINT signal to stop server

2. CLIENT 1
	1 use seperate terminal to run client program
	2. Type command < ./messenger_client client_configuration_file <PORT NO client will use to run> >
		ex ./messenger_client client_configuration_file 5800
	3. Press 'r' to register then 'l' to login
	4. follow 'm','ia' and 'i' structure to send message and make friend

3 CLEINT 2
	1 use seperate terminal to run client program
	2. Type command < ./messenger_client client_configuration_file <PORT NO client will use to run> >
		ex ./messenger_client client_configuration_file 5400
	3. Press 'r' to register then 'l' to login
	4. follow 'm','ia' and 'i' structure to send message and make friend

	 