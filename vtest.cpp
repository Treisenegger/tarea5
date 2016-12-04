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
#define FILEROUTE "./server.dns" //Route of file with dns information


using namespace std;

class association
{
	public:
		char name[50], value[50], type[50];
};


list<association> parseFile(char * route)
{
	char name[50], value[50], type[10];
	int ttl;
	FILE * fid;
	std::list<association> ass_list;

	while(1)
	{
		fid = fopen(route, "r");
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


int main(int argc, char const *argv[])
{
	list<association> l = parseFile(FILEROUTE);
	for (size_t i = 0; i < l.size(), i++) {
		cout << l[i];
	}
	return 0;
}