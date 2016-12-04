/*
    Simple udp server
*/
#include<stdio.h> //printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<unistd.h> //close();
#include<arpa/inet.h>
#include<sys/socket.h>
#include<list>
#include<bitset>
#include<string>
#include<iostream>
#include<sstream>
 
#define BUFLEN 512  //Max length of buffer
#define PORT 1029   //The port on which to listen for incoming data
#define FILEROUTE ".\server.dns" //Route of file with dns information

class association
{
	public:
		char name[50], value[50], type[50];
}

void die(char *s)
{
    perror(s);
    exit(1);
}

std::list<association> parseFile(route)
{
	char name[50], value[50], type[10];
	int ttl;
	FILE * fid;
	std::list<association> ass_list = {}

	while(1)
	{
		fid = fopen(route, "r");
		struct association next_ass = first_ass;
		while(!feof(fid))
		{
			fscanf(fid, "%s %s %s %d", name, value, type, &ttl);
			association ass;
			ass.name = name;
			ass.value = value;
			ass.type = type;
			ass_list.push_back(ass);
		}
	}
	return ass_list;
}

sdt::string getStringFromBits(bits)
{
	std::string line = "";
	for (int i = 96; i < bits.size(); i++)
	{
		line += bits[i].to_string()
	}
	std::istringstream in(line);
	std::bitset<8> bs;
	std::string myString = "";

	while(in >> bs)
		myString += char(bs.to_ulong());

	return myString;
}

int main(void)
{
    struct sockaddr_in si_me, si_other;
     
    int s, i, slen = sizeof(si_other) , recv_len;
    std::bitset<BUFLEN> buf;

    std::list<association> ass_list = parseFile(FILEROUTE);

    //create a UDP socket
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }
     
    // zero out the structure
    memset((char *) &si_me, 0, sizeof(si_me));
     
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
     
    //bind socket to port
    if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
    {
        die("bind");
    }
     
    //keep listening for data
    while(1)
    {
        printf("Waiting for data...");
        fflush(stdout);
         
        //try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
        {
            die("recvfrom()");
        }
         
        //print details of the client/peer and the data received
        printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        printf("Data: %s\n" , buf);
         
        //now reply the client with the same data
        if (sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_other, slen) == -1)
        {
            die("sendto()");
        }
    }
 
    close(s);
    return 0;
}
