//@Mayank Srivastava
//@This file handles the client configuration functions section


#include "messenger_client.h"

int main(int num_argument, char * arr_argument[])
{
	int local_portofserver,*local_detailofserver;
    pthread_t thread_id;
    list_vect_onlinefrnd.clear();
    struct sigaction loc_sigset;
	string str_hold_buff,local_pswdr,local_getmessgstr,local_serverhst;
    loc_sigset.sa_handler = clint_signal_handling;
    sigemptyset(&loc_sigset.sa_mask);
    loc_sigset.sa_flags = 0;
    sigaction(SIGINT, &loc_sigset, NULL);
    if (num_argument < 2){
        printf("Configuration file missing in input\n");
        exit(0);
    }
    config_file_reading(arr_argument[1],local_serverhst,local_portofserver);
    dif_server_sock=clinet_to_other_machine_conn(local_serverhst.c_str(),local_portofserver,true);
    if(num_argument==3){
        dif_servpeer_sock=atoi(arr_argument[2]);
    }
    local_detailofserver = (int *)malloc(sizeof(int));
    *local_detailofserver = dif_server_sock;
    pthread_create(&thread_id, NULL, &servconnwork, (void*)local_detailofserver);
	string local_sendingmessage="m ";
	string local_genterated_livestr;
	string local_sendingusr;
    string local_invitemessage="i ";
    string local_acceptinvitemessage="ia ";
    istringstream local_storagebufferformgs("");
	cout<<"WELCOME TO THE MESSENGER APP"<<endl;
    client_rcv_first_msg();
    while (getline(cin, str_hold_buff)){
        if(str_hold_buff.compare("r")==0){
            cout<<"Username:";
            cin>>username;
            cout<<"Password:";
            cin>>local_pswdr;
            local_getmessgstr=register_msg_generator_str(username,local_pswdr);
            write(dif_server_sock, local_getmessgstr.c_str(), strlen(local_getmessgstr.c_str())+1);
        }
        else if(str_hold_buff.compare("l")==0){
            if(stat_oflogin){
                cout<<"ALERT: User already logged in"<<endl;
                continue;
            }
            cout<<"Enter your Username:";
            cin>>username;
            cout<<"Enter your Password:";
            cin>>local_pswdr;
            local_getmessgstr=login_msg_geenrator_str(username,local_pswdr);
            write(dif_server_sock, local_getmessgstr.c_str(), strlen(local_getmessgstr.c_str())+1);
        }
        else if(str_hold_buff.compare("exit")==0){
            cllogout_fromserv(false);
            close(dif_server_sock);
            exit(0);
        }
        else if(str_hold_buff.compare("logout")==0){
            if(!stat_oflogin){
                cout<<"ALERT: Presently no user is as logged in"<<endl;
                continue;
            }
            cllogout_fromserv(true);

        }
        else{
            if(strncmp(str_hold_buff.c_str(),local_sendingmessage.c_str(),strlen(local_sendingmessage.c_str()))==0){
                local_storagebufferformgs.clear();
				str_hold_buff=str_hold_buff+"~";
                local_storagebufferformgs.str(str_hold_buff);
                getline(local_storagebufferformgs,local_sendingusr,' ');
                getline(local_storagebufferformgs,local_sendingusr,' ');
                getline(local_storagebufferformgs,local_genterated_livestr,'~');
                client2client_msg(local_sendingusr,local_genterated_livestr);
				}
            else if(strncmp(str_hold_buff.c_str(),local_invitemessage.c_str(),strlen(local_invitemessage.c_str()))==0){
                local_storagebufferformgs.clear();
				str_hold_buff=str_hold_buff+"~";
                local_storagebufferformgs.str(str_hold_buff);
                getline(local_storagebufferformgs,local_sendingusr,' ');
                getline(local_storagebufferformgs,local_sendingusr,' ');
                getline(local_storagebufferformgs,local_genterated_livestr,'~');
                local_getmessgstr=invite_msg_generator_str(local_sendingusr,local_genterated_livestr);
                write(dif_server_sock, local_getmessgstr.c_str(), strlen(local_getmessgstr.c_str())+1);
            }
            else if(strncmp(str_hold_buff.c_str(),local_acceptinvitemessage.c_str(),strlen(local_acceptinvitemessage.c_str()))==0){
                local_storagebufferformgs.clear();
				str_hold_buff=str_hold_buff+"~";
                local_storagebufferformgs.str(str_hold_buff);
                getline(local_storagebufferformgs,local_sendingusr,' ');
                getline(local_storagebufferformgs,local_sendingusr,' ');
                getline(local_storagebufferformgs,local_genterated_livestr,'~');

                auto itr=list_vect_usersinct.find(local_sendingusr);
                if(itr==list_vect_usersinct.end()){
                    cout<<"No invitation found from "<<local_sendingusr<<endl;
                    continue;
                }
                local_getmessgstr=invite_acpt_generator_str(local_sendingusr,local_genterated_livestr);
                write(dif_server_sock, local_getmessgstr.c_str(), strlen(local_getmessgstr.c_str())+1);
            }           
        }
    }
}

void client2client_msg(string p_user_to_user_frnd_name,string p_user_to_user_frnd_msg)
{
    auto iter=list_vect_onlinefrnd.find(p_user_to_user_frnd_name);
    if(iter != list_vect_onlinefrnd.end()){
        client_class class_var_userdetailfrnd=iter->second;
        if(!class_var_userdetailfrnd.isConnected()){
            int local_sockforfrnd=clinet_to_other_machine_conn(class_var_userdetailfrnd.ipaddress.c_str(),class_var_userdetailfrnd.listeningPort,false);
            if(local_sockforfrnd!=-1){
                class_var_userdetailfrnd.sockfd=local_sockforfrnd;
                iter->second=class_var_userdetailfrnd;
                thrrd_forsock(local_sockforfrnd);
                list_vect_csockpeer.push_back(local_sockforfrnd);
            }
            else{
                cout<<"ALERT: Connection error with peer"<<endl;
                onlinerequest_deletefrnd(class_var_userdetailfrnd.username);
                return;
            }
        }

        string loc_payload=seng_msg_generator_str(username,p_user_to_user_frnd_msg);
        write(class_var_userdetailfrnd.sockfd,loc_payload.c_str(),strlen(loc_payload.c_str())+1);
    }
    else{
        cout<<"NOTIFICATION: Your friend name "<<p_user_to_user_frnd_name<<" is OFFLINE"<<endl;
    }
}

void setdatatosock(int sockfd,string p_strcr)
{

    write(sockfd,p_strcr.c_str(), strlen(p_strcr.c_str())+1);
}

void c2server_socket_create(int p_portdet){
	pthread_t thread_id;
    struct sockaddr_in addr_serv;
	int loc_deschandler;
    loc_deschandler = socket(AF_INET, SOCK_STREAM, 0);

    bzero((void*)&addr_serv, sizeof(addr_serv));
    addr_serv.sin_family = AF_INET;
    addr_serv.sin_addr.s_addr = htonl(INADDR_ANY);
    addr_serv.sin_port = htons(p_portdet);

    bind(loc_deschandler, (struct sockaddr *)&addr_serv, sizeof(addr_serv));

    listen(loc_deschandler, 5);

    int * loc_sockedhpoint;
    loc_sockedhpoint = (int *)malloc(sizeof(int));
    *loc_sockedhpoint = loc_deschandler;
    pthread_create(&thread_id, NULL, &cliacpt_connection, (void*)loc_sockedhpoint);
}
void searver_msg_undstd(int p_disctripterofsock,char *p_data1)
{
    string local_data2buf(p_data1);
    istringstream local_buf2streammsg(local_data2buf);
    string local_messengerbdy;
	string local_fetchresp;
    getline(local_buf2streammsg,local_messengerbdy,'~');
    if(local_messengerbdy.compare("login")==0){
        getline(local_buf2streammsg,local_messengerbdy,'~');
        if(local_messengerbdy.compare("200")==0){
            stat_oflogin=true;
            cout<<"NOTIFICATION: you have been successfully logged in"<<endl;
            client_rcv_welcome_msg();
            c2server_socket_create(dif_servpeer_sock);
            setdatatosock(p_disctripterofsock,location_msg_genetator_str(username,dif_servpeer_sock));
        }
        else{
            cout<<"Retry: Login Failed"<<endl;
        }
    }
    else if(local_messengerbdy.compare("register")==0){
        getline(local_buf2streammsg,local_messengerbdy,'~');
        if(local_messengerbdy.compare("200")==0){
            cout<<"NOTIFICATION: You have been successfully registered"<<endl;

        }
        else{
            cout<<"ALERT: Retry: registration failed"<<endl;
        }
    }
    else if(local_messengerbdy.compare("location")==0){
        getline(local_buf2streammsg,local_messengerbdy,'~');
        cl_friend_loc_read(local_messengerbdy);
    }
    else if(local_messengerbdy.compare("invite")==0){
        getline(local_buf2streammsg,local_messengerbdy,'~');
        invit_msg_readi(local_messengerbdy);
    }
    local_buf2streammsg.clear();
}
void cl_friend_loc_read(string p_datamge){
    istringstream loc_bufferdatast(p_datamge);
	string loc_getrep,username;
	string	loc_addrremote;
	string	listetingport;
    istringstream loc_sfotchec("");
    while(getline(loc_bufferdatast,loc_getrep,'|')){
        loc_sfotchec.clear();
        loc_sfotchec.str(loc_getrep);
        getline(loc_sfotchec,loc_getrep,':');
        if(loc_getrep.compare("username")==0){
            getline(loc_sfotchec,username,':');
        }
        else if(loc_getrep.compare("ip")==0){
            getline(loc_sfotchec,loc_addrremote,':');
        }
        else if(loc_getrep.compare("port")==0){
            getline(loc_sfotchec,listetingport,':');
        }
    }
    cout<<"NOTIFICATION: Your friend "<<username<<" is available"<<endl;
    loc_bufferdatast.clear();
    loc_sfotchec.clear();
    client_class onlineFriend(username,loc_addrremote,stoi(listetingport));
    list_vect_onlinefrnd.insert({username,onlineFriend});
}
int clinet_to_other_machine_conn(const char *p_addr, int p_port,bool p_checker)
{
    int loc_check7,loc_getret;
	
	
    struct addrinfo hints, *res, *retssave;
    bzero(&hints, sizeof(struct addrinfo));
	int sockfd=-1;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((loc_getret = getaddrinfo(p_addr, to_string(p_port).c_str() , &hints, &res)) != 0){
        cout << "ALERT: wrong ip or port provided: " << gai_strerror(loc_getret) << endl;
        if(p_checker){
            exit(1);
        }
        return sockfd;
    }

    retssave = res;
    loc_check7 = 0;
    do{
        sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (sockfd < 0)
            continue;
        if (connect(sockfd, res->ai_addr, res->ai_addrlen) == 0){
            loc_check7 = 1;
            break;
        }
        close(sockfd);
    }
    while ((res = res->ai_next) != NULL);{
    freeaddrinfo(retssave);
	}

    if (loc_check7 == 0){
        fprintf(stderr, "ALERT: conection problem faced\n");
        if(p_checker)
        {
            exit(1);
        }
    }
    return sockfd;
}
void config_file_reading(char *loc_detailf, string &p_machinedetais, int &p_machineport){
    ifstream loc_datastore1;
    loc_datastore1.open(loc_detailf);
    string loc_dum;
	string loc_dum1;

    for(int i=0; i<2; i++){
        getline(loc_datastore1,loc_dum);
        istringstream lineStream(loc_dum);
        getline(lineStream,loc_dum1,':');

        if(loc_dum1.compare("server_host")==0){
            getline(lineStream,p_machinedetais,':');

        }
        else if(loc_dum1.compare("server_port")==0){
            getline(lineStream,loc_dum1,':');
            p_machineport=stoi(loc_dum1);
        }

    }
}
void invit_msg_readi(string p_data_msg)
{
    string l_stringss,fromuser, l_user_megiv;
    istringstream l_buffertoinput1(p_data_msg);
    istringstream l_stmav("");
    while(getline(l_buffertoinput1,l_stringss,'|')){
        l_stmav.clear();
        l_stmav.str(l_stringss);
        getline(l_stmav,l_stringss,':');
        if(l_stringss.compare("fromuser")==0){
            getline(l_stmav,fromuser,':');
        }
        else if(l_stringss.compare("message")==0){
            getline(l_stmav,l_user_megiv,':');
        }

    }

    cout<<"NOTIFICATION: Invitation from the user name: "<<fromuser<<endl;

    if(l_user_megiv.size()>1){
        cout<<fromuser;
        cout<<" ---> "<<l_user_megiv<<endl;
    }
    else{
        cout<<endl;
    }
    auto iter=list_vect_usersinct.find(fromuser);
    if(iter==list_vect_usersinct.end()){
        list_vect_usersinct.insert(fromuser);
    }
    l_buffertoinput1.clear();
    l_stmav.clear();
}
string login_msg_geenrator_str(string p_datadet, string p_datapd)
{
    ostringstream ltr_ptre;
    string ltr_datagen2;
    ltr_ptre<<"login"<<'~'<<"username"<<':'<<p_datadet<<'|'<<"password"<<':'<<p_datapd;
    ltr_datagen2=ltr_ptre.str();
    ltr_ptre.clear();
    return ltr_datagen2;
}

void cl_read_frnd_msg(string p_data1, int p_machindet){ 
	istringstream loc_bufstr(p_data1);
	string loc_dummy5;
	string username;
	string	message;
    istringstream loc_bufgrt("");
    getline(loc_bufstr,loc_dummy5,'~');
    if(loc_dummy5.compare("message")==0){
        while(getline(loc_bufstr,loc_dummy5,'|')){
            loc_bufgrt.clear();
            loc_bufgrt.str(loc_dummy5);
            getline(loc_bufgrt,loc_dummy5,':');
            if(loc_dummy5.compare("username")==0){
                getline(loc_bufgrt,username,':');
            }
            else if(loc_dummy5.compare("message")==0){
                getline(loc_bufgrt,message,':');
            }
        }
        cout<<username<<" ---> "<<message<<endl;
        loc_bufstr.clear();
        loc_bufgrt.clear();
        auto iter=list_vect_onlinefrnd.find(username);
        if(iter!=list_vect_onlinefrnd.end()){
            client_class onlineFriend=iter->second;
            if(onlineFriend.sockfd==-1){
                onlineFriend.sockfd=p_machindet;
            }
            iter->second=onlineFriend;
        }
    }
}
string location_msg_genetator_str(string p_datadet, int p_datacomm)
{
    ostringstream ltr_ptre;
    string ltr_datagen2;
    ltr_ptre<<"location"<<'~'<<"username"<<':'<<p_datadet<<'|'<<"port"<<':'<<to_string(p_datacomm);
    ltr_datagen2=ltr_ptre.str();
    ltr_ptre.clear();
    return ltr_datagen2;
}

string register_msg_generator_str(string p_datadet, string p_datapd)
{
    ostringstream ltr_ptre;
    string ltr_datagen2;
    ltr_ptre<<"register"<<'~'<<"username"<<':'<<p_datadet<<'|'<<"password"<<':'<<p_datapd;
    ltr_datagen2=ltr_ptre.str();
    ltr_ptre.clear();
    return ltr_datagen2;
}


string invite_acpt_generator_str(string p_datadet, string p_data1){
    ostringstream ltr_ptre;
    string ltr_datagen2;
    ltr_ptre<<"accept_invite"<<'~'<<"fromuser"<<':'<<username<<'|'<<"touser"<<':'<<p_datadet<<'|'<<"message"<<':'<<p_data1;
    ltr_datagen2=ltr_ptre.str();
    ltr_ptre.clear();
    return ltr_datagen2;;
}
void onlinerequest_deletefrnd(string p_machna){
    client_class onlineFriend;
    auto iter=list_vect_onlinefrnd.find(p_machna);
    if(iter!=list_vect_onlinefrnd.end()){
        iter=list_vect_onlinefrnd.erase(iter);
        cout<<"NOTIFICATION: your friend"<<p_machna<<" is now available(offline)"<<endl;
        return;
    }
}

void onlinerequest_deletefrnd(int p_ger)
{
    client_class obj_detailson;
    auto iter=list_vect_onlinefrnd.begin();
    while(iter!=list_vect_onlinefrnd.end()){
        obj_detailson=iter->second;
        if(obj_detailson.sockfd==p_ger){
            iter=list_vect_onlinefrnd.erase(iter);
            cout<<"Friend "<<obj_detailson.username<<" is offline"<<endl;
            return;
        }
        ++iter;
    }
}
void *proc_create(void *val_passde){
    int descofsoc;
    ssize_t counrt;
    char buf[4096];
    string bfrstr;
    descofsoc = *((int *)val_passde);
    free(val_passde);

    pthread_detach(pthread_self());
    while ((counrt = read(descofsoc, buf, 4096)) > 0){
        buf[counrt] = '\0';
        bfrstr=buf;
        cl_read_frnd_msg(bfrstr,descofsoc);
    }
    if (counrt == 0){
        cout << "ALERT: Peer connection is closed" << endl;

    }
    else{
        cout << "ALERT: improper functioning" << endl;
    }
    onlinerequest_deletefrnd(descofsoc);
    close(descofsoc);
    return(NULL);
}
void client_rcv_welcome_msg()
{
	cout<<endl<<"************** GENERAL INSTRUCTIONS ***********"<<endl;
	cout<<"Type following command to accept invitation 'ia inviter_username' "<<endl;
    cout<<"Type following command to send message : 'm friend_username message' "<<endl;
    cout<<"Type following command to invite for chat'i potential_friend_username message' "<<endl;
	cout<<"Type following command to exit from the server'logout'"<<endl;
}
string invite_msg_generator_str(string p_datadet, string p_data1)
{
    ostringstream ltr_ptre;
    string ltr_datagen2;
    ltr_ptre<<"invite"<<'~'<<"fromuser"<<':'<<username<<'|'<<"touser"<<':'<<p_datadet<<'|'<<"message"<<':'<<p_data1;
    ltr_datagen2=ltr_ptre.str();
    ltr_ptre.clear();
    return ltr_datagen2;
}

void *cliacpt_connection(void *val_pas)
{
    struct sockaddr_in local_data23;
    socklen_t local_datasv23;
    int local_machindic;
    int local_machineserverf;

    local_machineserverf = *((int *)val_pas);
    free(val_pas);

    for (; ;){
		//cout<<"debug cliacpt_connection"<<endl;
        local_datasv23 = sizeof(local_data23);
        local_machindic = accept(local_machineserverf, (struct sockaddr *)&local_data23, &local_datasv23);
        list_vect_csockpeer.push_back(local_machindic);
		thrrd_forsock(local_machindic);
    }
    return(NULL);
}

void clint_signal_handling(int signo)
{
    cout<<endl<<"SIGINT sinal is generated"<<endl;
    cllogout_fromserv(false); 
    close(dif_server_sock);
    exit(0);
}


void thrrd_forsock(int p_py){
    int *loc_machinegg;
    loc_machinegg = (int *)malloc(sizeof(int));
    *loc_machinegg = p_py;
    pthread_t thred_id;
	//cout<<"degub thrrd_forsock"<<endl;
    pthread_create(&thred_id, NULL, &proc_create, (void*)loc_machinegg);
}

string seng_msg_generator_str(string p_datadet, string p_data1)
{
    ostringstream ltr_ptre;
    string ltr_datagen2;
    ltr_ptre<<"message"<<'~'<<"username"<<':'<<p_datadet<<'|'<<"message"<<':'<<p_data1;
    ltr_datagen2=ltr_ptre.str();
    ltr_ptre.clear();
    return ltr_datagen2;
}
void cllogout_fromserv(bool p_klir){
    auto itar = list_vect_csockpeer.begin();
    while (itar != list_vect_csockpeer.end()){
        int fd;
        fd = *itar;
        close(fd);
        ++itar;
    }
    list_vect_onlinefrnd.clear();
    setdatatosock(dif_server_sock,logout_msg_generator_str());
    stat_oflogin=false;
    if(p_klir){
         client_rcv_first_msg();
    }
}

void client_rcv_first_msg()
{
	cout<<endl<<"*************BASIC STEPS****************"<<endl;
    cout<<"Type 'r' to register"<<endl;
    cout<<"Type 'l' to Login"<<endl;
}



void *servconnwork(void *arg)
{
    int n;
    int sockfd;
    char buf[4096];
    sockfd = *((int *)arg);
    free(arg);
    pthread_detach(pthread_self());
    while (1)
    {
        n = read(sockfd, buf, 4096);
        if (n == 0)
        {
            printf("Server Crashed!!!\n");
            close(sockfd);
            exit(0);
        }
        searver_msg_undstd(sockfd, buf);
    }
}

string logout_msg_generator_str()
{
    ostringstream ltr_ptre;
    string ltr_datagen2;
    ltr_ptre<<"logout"<<'~'<<"username"<<':'<<username;
    ltr_datagen2=ltr_ptre.str();
    ltr_ptre.clear();
    return ltr_datagen2;;
}
