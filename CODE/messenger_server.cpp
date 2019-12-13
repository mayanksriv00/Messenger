/*
****@Mayank Srivastava
****@THis is the cpp file of the messernger_server that contains only the functions

*/


#include "messenger_server.h"



void server_notify_client_r3(int descrip_filea){
    ser_class2 loc_clasobj;
    auto iter=server_fulllistronline.begin();
    while(iter!=server_fulllistronline.end()){
        loc_clasobj=iter->second;
        if(loc_clasobj.sockfd==descrip_filea){
            iter=server_fulllistronline.erase(iter);
            cout<<"online users"<<server_fulllistronline.size()<<endl;
            return;
        }
        ++iter;
    }
}
bool server_undst_user_msg(int p_disp_detp,char *p_data)
{
    string convto_st(p_data);
    istringstream loc_varistr(convto_st);
    string loc_datamain,loc_getretcode;
    cout<<"Client:"<<convto_st<<endl<<flush;
    getline(loc_varistr,loc_datamain,'~');
    if(loc_datamain.compare("login")==0)
	{
        getline(loc_varistr,loc_datamain,'~');
        if(server_notify_clientlogin(loc_datamain))
		{
            loc_getretcode=server_working_basic_string1("login","200");
        }
        else{
            loc_getretcode=server_working_basic_string1("login","500");
        }
        write(p_disp_detp,loc_getretcode.c_str(),strlen(loc_getretcode.c_str())+1);
    }
    else if(loc_datamain.compare("register")==0)
	{
        getline(loc_varistr,loc_datamain,'~');
        if(server_recev_userrrequest(loc_datamain)){
            loc_getretcode=server_working_basic_string1("register","200");
        }
        else{
            loc_getretcode=server_working_basic_string1("register","500");
        }
        write(p_disp_detp,loc_getretcode.c_str(),strlen(loc_getretcode.c_str())+1);
    }
    else if(loc_datamain.compare("location")==0)
	{
        getline(loc_varistr,loc_datamain,'~');
        server_send_locatq(p_disp_detp,loc_datamain);
    }
    else if(loc_datamain.compare("invite")==0)
	{
        getline(loc_varistr,loc_datamain,'~');
        server_pass_invtr2(loc_datamain);
    }
    else if(loc_datamain.compare("accept_invite")==0)
	{
        getline(loc_varistr,loc_datamain,'~');
        server_get_invq2(loc_datamain);
    }
    else if(loc_datamain.compare("logout")==0)
	{
        getline(loc_varistr,loc_datamain,'~');
        server_client_exitdet(loc_datamain);
        return true;
    }
    loc_varistr.clear();
    return false;
}
int main(int p_count_ar, char * p_array_ar[])
{
    server_fulllistronline.clear();
    socklen_t len;
    struct sockaddr_in loc_adrss1, loc_recalasde2;
    struct sigaction loc_sig_set_store;
	
    char loc_data_hold[4096];
	int loc_sockdesc12;
    fd_set rset;
	int loc_sock_port_det2;
    
    loc_sig_set_store.sa_handler = sig_int;
    sigemptyset(&loc_sig_set_store.sa_mask);
    loc_sig_set_store.sa_flags = 0;
    sigaction(SIGINT, &loc_sig_set_store, NULL);
    read_file_for_details3(p_array_ar[1]);
    loc_sock_port_det2=read_file_for_details2(p_array_ar[2]);
    if (p_count_ar < 3)    {
        cout<<"Configuration file missing please follow the format"<<endl;
        exit(0);
    }
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
        perror("Alert: unable to find the socket details");
        exit(1);
    }
    memset(&loc_adrss1, 0, sizeof(struct sockaddr_in));
    loc_adrss1.sin_addr.s_addr = INADDR_ANY;
    loc_adrss1.sin_family = AF_INET;
    loc_adrss1.sin_port = htons((short)loc_sock_port_det2);
    if (bind(sockfd, (struct sockaddr *)&loc_adrss1, sizeof(loc_adrss1)) < 0){
        perror(": bind");
        exit(1);
    }
    char hostname[128];
    if(gethostname(hostname,128)==-1){
        perror("Failed to get hostname");
        exit(EXIT_FAILURE);
    }
    struct hostent *host;
    host=gethostbyname(hostname);
    cout<<"Server is starting ..."<<endl;
	cout<<"Server is started ...."<<endl;
    cout<<"Server details:"<<host->h_name;
    len = sizeof(loc_adrss1);
    if (getsockname(sockfd, (struct sockaddr *)&loc_adrss1, &len) < 0) {
      perror("unable to find name");
      _exit(1);
   }
     printf("\nPort:%d\n", htons(loc_adrss1.sin_port));
    if (listen(sockfd, 5) < 0){
        perror(": bind");
        exit(1);
    }
    FD_ZERO(&allset);
    FD_SET(sockfd, &allset);
    loc_sockdesc12 = sockfd;
    server_listpostso.clear();
    while (1){
        rset = allset;
        select(loc_sockdesc12+1, &rset, NULL, NULL, NULL);
        if (FD_ISSET(sockfd, &rset)){
            len = sizeof(loc_recalasde2);
            if ((rec_sock = accept(sockfd, (struct sockaddr *)(&loc_recalasde2), &len)) < 0){
                if (errno == EINTR)
                    continue;
                else{
                    perror("Alert: Error acception");
                    exit(1);
                }
            }
            if (rec_sock < 0){
                perror(": accept");
                exit(1);
            }
           printf("Client Machine details= %s | Port = %d.\n",inet_ntoa(loc_recalasde2.sin_addr), ntohs(loc_recalasde2.sin_port));
            server_listpostso.push_back(rec_sock);
            FD_SET(rec_sock, &allset);
            if (rec_sock > loc_sockdesc12) loc_sockdesc12 = rec_sock;
        }
        auto iter = server_listpostso.begin();
        while (iter != server_listpostso.end()){
            int coun;
			int desc_soct;
            desc_soct = *iter;
            if (FD_ISSET(desc_soct, &rset)){
                coun = read(desc_soct, loc_data_hold, 4096);
                if (coun == 0){
                    close(desc_soct);
                    FD_CLR(desc_soct, &allset);
                    iter = server_listpostso.erase(iter);
                    server_notify_client_r3(desc_soct);
                    continue;
               }
                else{
                    bool curr_loddet=server_undst_user_msg(desc_soct,loc_data_hold);
                    if(curr_loddet){
                        continue;
                    }
                }
            }
            ++iter;
        }
        loc_sockdesc12 = sockfd;
        if (!server_listpostso.empty()){
            loc_sockdesc12 = max(loc_sockdesc12, *max_element(server_listpostso.begin(), server_listpostso.end()));
        }
    }
}
void server_client_exitdet(string bodymsfg)
{
    string loc_stringhold;
	string username;
    istringstream loc_stream2(bodymsfg);
    istringstream loc_stream1("");
    while(getline(loc_stream2,loc_stringhold,'|'))
    {
        loc_stream1.clear();
        loc_stream1.str(loc_stringhold);
        getline(loc_stream1,loc_stringhold,':');
        if(loc_stringhold.compare("username")==0)
        {
            getline(loc_stream1,username,':');
        }

    }
    auto iter=server_fulllistronline.find(username);
    if(iter!=server_fulllistronline.end())
    {
        ser_class2 loggedOutUser=iter->second;
        server_fulllistronline.erase(iter);
        cout<<"online user list:"<<server_fulllistronline.size()<<endl;

    }
}
void server_get_invq2(string p_body_msg)
{
    string fromuser,loc_genmsg;
	string touser;
	string loc_dummystr;
    istringstream loc_stream1(p_body_msg);
    istringstream loc_stream2("");
    while(getline(loc_stream1,loc_dummystr,'|'))
	{
        loc_stream2.clear();
        loc_stream2.str(loc_dummystr);
        getline(loc_stream2,loc_dummystr,':');
        if(loc_dummystr.compare("touser")==0)
		{
            getline(loc_stream2,touser,':');
        }
        else if(loc_dummystr.compare("fromuser")==0)
		{
            getline(loc_stream2,fromuser,':');
        }
    }
    loc_stream1.clear();
    loc_stream2.clear();
    ostringstream buffer;
    buffer<<"accept_invite"<<'~'<<p_body_msg;
    loc_genmsg=buffer.str();
    buffer.clear();
    auto iter=server_fulllistronline.find(touser);
    if(iter!=server_fulllistronline.end())
	{
        ser_class2 inviter=iter->second;
        inviter.user.frnd_datal.push_back(fromuser);
        iter->second=inviter;
        write(inviter.sockfd,loc_genmsg.c_str(),strlen(loc_genmsg.c_str())+1);
        auto itr=server_fulllistronline.find(fromuser);
        ser_class2 invitee=itr->second;
        invitee.user.frnd_datal.push_back(touser);
        itr->second=invitee;
        auto i=server_fulllistusr.find(touser);
        server_cl1 inviterUser=i->second;
        inviterUser.frnd_datal.push_back(fromuser);
        i->second=inviterUser;
        i=server_fulllistusr.find(fromuser);
        server_cl1 inviteeUser=i->second;
        inviteeUser.frnd_datal.push_back(touser);
        i->second=inviteeUser;
        sleep(1);
        loc_genmsg=paylocstrwe(inviter.user.username,inviter.ip,to_string(inviter.listeningPort));
        write(invitee.sockfd,loc_genmsg.c_str(),strlen(loc_genmsg.c_str())+1);
        loc_genmsg=paylocstrwe(invitee.user.username,invitee.ip,to_string(invitee.listeningPort));
        write(inviter.sockfd,loc_genmsg.c_str(),strlen(loc_genmsg.c_str())+1);
    }
}
void server_save_detaild(){
    ofstream loc_usrfiledefnane ("user_info_file");
    server_cl1 user;
    if (loc_usrfiledefnane.is_open())
	{
        auto iter=server_fulllistusr.begin(); 
        while(iter!=server_fulllistusr.end()){
            user=iter->second;
            loc_usrfiledefnane<<user.username<<"|"<<user.password<<"|";
            auto itr=user.frnd_datal.begin();
            while(itr!=user.frnd_datal.end()){
                loc_usrfiledefnane<<*itr;
                ++itr;
                if(itr!=user.frnd_datal.end()){
                    loc_usrfiledefnane<<";";
                }
            }
            loc_usrfiledefnane<<"\n";
            ++iter;
        }
        loc_usrfiledefnane.close();
    }
}


void server_send_locatq(int sockfd,string p_databr)
{
    cout<<"location send of user"<<endl;
	
	
    string loc_dumm1, loc_portsd;
	string prdrf,username;

	string	ipaddress;
    istringstream loc_stream1(p_databr);
    istringstream loc_stream2("");
    while(getline(loc_stream1,loc_dumm1,'|'))
	{
        loc_stream2.clear();
        loc_stream2.str(loc_dumm1);
        getline(loc_stream2,loc_dumm1,':');
        if(loc_dumm1.compare("username")==0){
            getline(loc_stream2,username,':');
        }
        else if(loc_dumm1.compare("port")==0){
            getline(loc_stream2,loc_portsd,':');
        }
    }
    ipaddress=server_fetch_client_sockdet(sockfd);
    cout<<"user ="<<username<<" address ="<<ipaddress<<" listening port ="<<loc_portsd<<endl;
    loc_stream1.clear();
    loc_stream2.clear();
    prdrf=paylocstrwe(username,ipaddress,loc_portsd);
    string payloadOfrndLocation;
    auto iter=server_fulllistusr.find(username);
    if(iter != server_fulllistusr.end()){
        server_cl1 user=iter->second;
        auto itr = user.frnd_datal.begin();
        while (itr != user.frnd_datal.end()){
            server_cl1 friendUser=*itr;
            auto frnd=server_fulllistronline.find(friendUser.username);
            if(frnd!=server_fulllistronline.end()){
                ser_class2 onlinefriend=frnd->second;
                cout<<"online friends"<<friendUser.username<<endl;
                write(onlinefriend.sockfd,prdrf.c_str(),strlen(prdrf.c_str())+1);
                payloadOfrndLocation=paylocstrwe(onlinefriend.user.username,onlinefriend.ip,to_string(onlinefriend.listeningPort));
                write(sockfd,payloadOfrndLocation.c_str(),strlen(payloadOfrndLocation.c_str())+1);
            }
            ++itr;
		}
        ser_class2 onlineUser(sockfd,user,ipaddress,stoi(loc_portsd));
        server_fulllistronline.insert({user.username,onlineUser});
        cout<<"users online count list"<<server_fulllistronline.size()<<endl;
    }
}
string server_fetch_client_sockdet(int loc_descr){
    string peerAddress;
    struct sockaddr_in loc_lo;
    socklen_t len;
    memset(&loc_lo, 0, sizeof(loc_lo));
    len=sizeof(loc_lo);
    getpeername( loc_descr,(struct sockaddr *) &loc_lo,&len);
    peerAddress=inet_ntoa(loc_lo.sin_addr);
    cout<<"address details"<<peerAddress<<endl;
    return peerAddress;

}
string paylocstrwe(string p_name, string p_add, string p_port){
    ostringstream loc_datahold;
    string loc_gensrt;
    loc_datahold<<"location"<<'~'<<"username"<<':'<<p_name<<'|'<<"port"<<':'<<p_port<<'|'<<"ip"<<':'<<p_add;
    loc_gensrt=loc_datahold.str();
    loc_datahold.clear();
    return loc_gensrt;

}

void read_file_for_details3(char *p_locname){
    ifstream loc_filestreamer2;
    loc_filestreamer2.open(p_locname);
    server_fulllistusr.clear();
    string loc_dumstr5;
    while(getline(loc_filestreamer2,loc_dumstr5)){
        server_cl1 user(loc_dumstr5);
        server_fulllistusr.insert({user.username,user});
    }
    loc_filestreamer2.close();
}
void server_pass_invtr2(string p_msglocal)
{
    string loc_dummyst,touser;
	
	string loadmsmrinvtfr,loc_msgbuffr;
    istringstream loc_Stream1(p_msglocal);
    istringstream loc_Stream2("");
    while(getline(loc_Stream1,loc_dummyst,'|'))
	{
        loc_Stream2.clear();
        loc_Stream2.str(loc_dummyst);
        getline(loc_Stream2,loc_dummyst,':');
        if(loc_dummyst.compare("touser")==0){
            getline(loc_Stream2,touser,':');
        }
    }
    loc_Stream1.clear();
    loc_Stream2.clear();
    ostringstream buffer;
    buffer<<"invite"<<'~'<<p_msglocal;
    loc_msgbuffr=buffer.str();
    buffer.clear();
    auto iter=server_fulllistronline.find(touser);
    if(iter!=server_fulllistronline.end()){
        ser_class2 invitedUser=iter->second;
        write(invitedUser.sockfd,loc_msgbuffr.c_str(),strlen(loc_msgbuffr.c_str())+1);
    }
}
string server_working_basic_string1(string p_tpr,string p_stat){
    ostringstream loc_datahold;
    string loc_gensrt;
    loc_datahold<<p_tpr<<'~'<<p_stat;
    loc_gensrt=loc_datahold.str();
    loc_datahold.clear();
    return loc_gensrt;
}
int read_file_for_details2(char *pre_fildcof){
    ifstream loc_filestreamer2;
    loc_filestreamer2.open(pre_fildcof);
    string loc_dumstr5;
    getline(loc_filestreamer2,loc_dumstr5,':');
    if(loc_dumstr5.compare("port")==0)
	{
        getline(loc_filestreamer2,loc_dumstr5,':');
    }
    else
	{
        cout<<"port not found conf"<<flush;
        exit(1);
    }

    loc_filestreamer2.close();
    return stoi(loc_dumstr5);
}
bool server_notify_clientlogin(string p_msgdata2)
{
    string p_holddum,password,username;
    istringstream loc_stream1(p_msgdata2);
    istringstream loc_Stream2("");
    while(getline(loc_stream1,p_holddum,'|')){
        loc_Stream2.clear();
        loc_Stream2.str(p_holddum);
        getline(loc_Stream2,p_holddum,':');
        if(p_holddum.compare("username")==0){
            getline(loc_Stream2,username,':');
        }
        else if(p_holddum.compare("password")==0){
            getline(loc_Stream2,password,':');
        }
    }
    loc_stream1.clear();
    loc_Stream2.clear();
    auto iter=server_fulllistusr.find(username);
    if(iter != server_fulllistusr.end()){
        server_cl1 user=iter->second;
        if(user.password.compare(password)==0)
		{
            cout<<username<<" login is successful"<<endl;
            return true;
        }
        else{
            cout<<"login is failed"<<endl;
        }
    }
    else{
        cout<<"failed login"<<endl;
    }
    return  false;
}
bool server_recev_userrrequest(string p_msgdata2){   
	string p_holddum,password,username;
    istringstream loc_Stream1(p_msgdata2);
    istringstream loc_stream2("");
    while(getline(loc_Stream1,p_holddum,'|')){
        loc_stream2.clear();
        loc_stream2.str(p_holddum);
        getline(loc_stream2,p_holddum,':');
        if(p_holddum.compare("username")==0){
            getline(loc_stream2,username,':');
        }
        else if(p_holddum.compare("password")==0){
            getline(loc_stream2,password,':');
        }
    }
    loc_Stream1.clear();
    loc_stream2.clear();
    cout<<"user :"<<username<<" pass: "<<password<<endl;
    auto iter=server_fulllistusr.find(username);
    if(iter != server_fulllistusr.end()){
        cout<<"registraing failed"<<endl;
    }
    else{
        cout<<"registration success"<<endl;
        server_cl1 user(username,password);
        server_fulllistusr.insert({user.username,user});
        return true;
    }
    return  false;
}
void sig_int(int signo)
{
    cout<<endl<<"SIGINT"<<endl;

    server_save_detaild();

    auto itr = server_listpostso.begin();
    while (itr != server_listpostso.end())
    {
        int loc_ifd;
        loc_ifd = *itr;
        close(loc_ifd);
        ++itr;
    }

    close(sockfd);
    exit(0);
}

