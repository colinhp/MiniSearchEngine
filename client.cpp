#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <json/json.h>
#include <string>
#include <iostream>
using std::string;
using std::cout;
using std::endl;

#define ERR_EXIT(m) \
    do { \
        perror(m);\
        exit(EXIT_FAILURE);\
    }while(0)

void do_service(int sockfd);
void parse_json(string &);

int main()
{
    int peerfd = socket(PF_INET, SOCK_STREAM, 0);
    if(peerfd == -1)
        ERR_EXIT("socket");

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("192.168.13.128"); //localhost
    //addr.sin_addr.s_addr = INADDR_ANY; //localhost
    addr.sin_port = htons(9999);
    socklen_t len = sizeof addr;
    if(connect(peerfd, (struct sockaddr*)&addr, len) == -1)
        ERR_EXIT("Connect");

	char buf[1024];
	memset(buf, 0, sizeof(buf));
	read(peerfd, buf, sizeof(buf));
	printf("%s\n", buf);

    do_service(peerfd);


    return 0;
}

void parse_json(string & str)
{
	Json::Value root;
	Json::Value elemArr;
	Json::Reader reader;
	if(!reader.parse(str,root,false)){
		return;	
	}
	
	int size = root["files"].size();
	cout << "来自服务器端的JSON字符串内的元素个数 size = " << size << endl;
	elemArr = root["files"];

	string summary;
	string title;
	string url;

	for(int i = 0; i < size; i++){
		title = elemArr[i]["title"].asString();
		url = elemArr[i]["url"].asString();
		summary = elemArr[i]["summary"].asString();		
		cout << "title: " << title <<endl;
		cout << "url : " << url <<endl;
		cout << "summary : " << summary <<endl;
		cout << "-----------------" << endl;
	}	
}

void do_service(int sockfd)
{
    char recvbuf[65535] = {0};
    char sendbuf[65535] = {0};
    while(1)
    {
        fgets(sendbuf, sizeof sendbuf, stdin);
        write(sockfd, sendbuf, strlen(sendbuf));

        //read
        int nread = read(sockfd, recvbuf, sizeof recvbuf);
        if(nread == -1)
        {
            if(errno == EINTR)
                continue;
            ERR_EXIT("read");
        }
        else if(nread == 0)
        {
            printf("server close!\n");
            close(sockfd);
            exit(EXIT_SUCCESS);
        }

//        printf("receive msg : %s\n", recvbuf);

		string ret = string(recvbuf);
		parse_json(ret);

        memset(recvbuf, 0, sizeof recvbuf);
        memset(sendbuf, 0, sizeof sendbuf);
    }
}




