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

class association
{
	public:
		std::string name, value, type;
};

void die(char *s)
{
    perror(s);
    exit(1);
}

std::list<association> parseFile(char * route)
{
	std::string name, value, type;
	int ttl;
	FILE * fid;
	std::list<association> ass_list = {};

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

int main(void)
{
    struct sockaddr_in si_me, si_other;
     
    int s, i, slen = sizeof(si_other) , recv_len;
    std::string buf;
    std::string recv_message, send_message, full_message;
    std::list<association> ass_list = parseFile(FILEROUTE);
    association send_ass;

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
    	std::string header;
        fflush(stdout);
         
        //try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
        {
            die("recvfrom()");
        }

	//El mensaje en sí está después del header que tiene largo 96
	recv_message = buf.substr(96);
	//Los primeros 16 elementos son el identificador del mensaje que es igual al del mensaje entrante
	header = buf.substr(0,16);
	//El siguiente elemento es un 1, dado que es una response
	header += "1";
	//Los siguientes 4 elementos dependen del tipo de query y son igual nelse  mnsaje entrante
	header += buf.substr(17,4);
	//En found guardaremos si encontramos la asociación
	int found = 0;
	//Si el elemento en la posición 18 es un 1 significa que es una reverse query
	if (buf.substr(18,1) == "1") {
		//Recorremos la lista buscando la asociación
		for (int i = 0; i < ass_list.size(); i++) {
			//Si encontramos la asociación decimos que nuestro mensaje será NAME,VALUE,TYPE
			if (ass_list[i].value == recv_message) {
				send_message = ass_list[i].name + "," + ass_list[i].value + "," + ass_list[i].type;
				//El siguiente bit del header es 1 si la asociación es authoritative
				if (type == "A")
					header += "1";
				else
					header += "0";
				found = 1;
				break
			}
		}
	}
	else {
		for (int i = 0; i < ass_list.size(); i++) {
			if (ass_list[i].name == recv_message) {
				send_message = ass_list[i].name + "," + ass_list[i].value + "," + ass_list[i].type;
				if (type == "A")
					header += "1";
				else
					header += "0";
				found = 1;
				break
			}
		}
	}
	//Si no se encuentra el hostname que se estaba buscando enviamos un mensaje que indique que no se encontró
	if (!found) {
		send_message = "NONE";
		header += "0";
	}
	//Los siguientes 6 bits son 0
	header += "000000";
	//Nuestro código de éxito es 0000 y el de fracaso es 1111
	if (found)
		header += "0000";
	else
		header += "1111";

	//Los siguientes bits dependen de casi nada
	header += "0000000000000001";
	header += "0000000000000001";
	header += "0000000000000000";
	header += "0000000000000000";

	//Juntamos header con data para formar el mensaje completo
	full_message = header + send_message;

        //now reply the client with the same data
        if (sendto(s, full_message, recv_len, 0, (struct sockaddr*) &si_other, slen) == -1)
        {
            die("sendto()");
        }
    }
 
    close(s);
    return 0;
}
