/*
*****@ Mayank Srivastava
*****@ This is the header of the messender_server.h file
*/


/*headers*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <sstream>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <errno.h>
#include <signal.h>
#include <fstream>
#include <netdb.h>
#include <algorithm>
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <unistd.h>


using namespace std;



class server_cl1
{
public:
    string username;
    vector<string> frnd_datal;
	string password;
    server_cl1(string param_f1)
	{
        string str;
		string config_file_fetchfr;
		
        istringstream f(param_f1);
        getline(f, username, '|');
        getline(f, password, '|');
        getline(f, config_file_fetchfr, '|');
		
        istringstream frnd_buff3(config_file_fetchfr);

        while (getline(frnd_buff3, str, ';'))
		{
            frnd_datal.push_back(str);
        }
    }
	 server_cl1()
	{
        username="";
        frnd_datal.clear();
		password="";
    }
    server_cl1(string p_det_user, string p_det_pasd)
	{
        username=p_det_user;
        password=p_det_pasd;
        frnd_datal.clear();
    }

};

class ser_class2
{
public:
    server_cl1 user;
	int listeningPort=-1;
    string ip;
	int sockfd;
    ser_class2()
	{
		
    }
    ser_class2(int conf_det1, server_cl1 conf_det2)
	{
		sockfd=conf_det1;
        user=conf_det2;
    }
    ser_class2(int conf_det1, server_cl1 conf_det2, string ipaddr, int port){
       
		ip=ipaddr;
	   user=conf_det2;
                
        listeningPort=port;
		sockfd=conf_det1;
    }
    void setIp(string conf_ipr)
	{
        this->ip=conf_ipr;
    }
    void setListeningPort(int conf_3)
	{
        listeningPort=conf_3;
    }
};




void server_get_invq2(string);
void server_client_exitdet(string);
bool server_undst_user_msg(int sec1,char *data2);
void server_save_detaild();
int read_file_for_details2(char * inputparam1);
void sig_int(int);
void server_send_locatq(int,string);
bool server_notify_clientlogin(string);
bool server_recev_userrrequest(string);
void server_notify_client_r3(int);
void read_file_for_details3(char *inputptr);
void server_pass_invtr2(string);

string server_working_basic_string1(string,string);
string server_fetch_client_sockdet(int);
string paylocstrwe(string,string,string);
string sever_und_datastr2(string,string);
unordered_map<string,server_cl1> server_fulllistusr;
unordered_map<string,ser_class2> server_fulllistronline;
int sockfd, rec_sock;
vector<int> server_listpostso;
fd_set allset;