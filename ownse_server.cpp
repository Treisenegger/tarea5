/*
    Simple udp server
*/
#include <stdio.h> //printf
#include <string.h> //memset
#include <stdlib.h> //exit(0);
#include <unistd.h> //close();
#include <arpa/inet.h>
#include <sys/socket.h>
#include <list>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
 
#define BUFLEN 512  //Max length of buffer
#define PORT 1029   //The port on which to listen for incoming data
#define FILEROUTE "./server.dns" //Route of file with dns information

using namespace std;

class association
{
	public:
		string name, value, type;
		size_t ttl;
};

void die(char *s)
{
    perror(s);
    exit(1);
}

list<string> split(string s, string sep) {

	/*Splits a string on spaces.*/

	list<string> l;

	size_t i = 0, pos = 0;

	while (pos != string::npos) {

		pos = s.find(sep, i);
		l.push_back(s.substr(i, pos - i));
		i = pos + 1;
	}

	return l;

}

list<association> parseFile(char * route)
{
	list<association> ass_list;
	list<string> l;


	string line;
	ifstream server_data (route);


	if (server_data.is_open()) {
    	while ( getline (server_data,line) ) {
    		l = split(line, " ");

    		association ass;
    		ass.name = l.front();
    		l.pop_front();
    		ass.value = l.front();
    		l.pop_front();
    		ass.type = l.front();
    		l.pop_front();
    		ass.ttl = stoi(l.front());
    		l.pop_front();

    		ass_list.push_back(ass);
    	}
    	server_data.close();
  	}

  	else cout << "Unable to open file"; 

  	return ass_list;
}

int main(void)
{
    struct sockaddr_in si_me, si_other;
     
    int s, i, recv_len;
    socklen_t slen = sizeof(si_other);
    string buf;
    string recv_message, send_message, full_message;
    list<association> ass_list = parseFile(FILEROUTE);

    cout << "Associations:\n";
  	for (list<association>::iterator it = ass_list.begin(); it != ass_list.end(); it++) {
  		cout << '\t' << (*it).name << " " << (*it).value << " " << (*it).type << " " << to_string((*it).ttl) << '\n';
  	}
  	cout << "\n\n";

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
    while("The world exists")
    {
    	string header;
        fflush(stdout);
         
        //try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(s, &buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
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
		for (list<association>::iterator it = ass_list.begin(); it != ass_list.end(); it++) {
			//Si encontramos la asociación decimos que nuestro mensaje será NAME,VALUE,TYPE
			if ((*it).value == recv_message) {
				send_message = (*it).name + "," + (*it).value + "," + (*it).type + "," + to_string((*it).ttl);
				//El siguiente bit del header es 1 si la asociación es authoritative
				if ((*it).type == "A")
					header += "1";
				else
					header += "0";
				found = 1;
				break;
			}
		}
	}
	else {
		//Recorremos la lista buscando la asociación
		for (list<association>::iterator it = ass_list.begin(); it != ass_list.end(); it++) {
			//Si encontramos la asociación decimos que nuestro mensaje será NAME,VALUE,TYPE
			if ((*it).name == recv_message) {
				send_message = (*it).name + "," + (*it).value + "," + (*it).type + "," + to_string((*it).ttl);
				//El siguiente bit del header es 1 si la asociación es authoritative
				if ((*it).type == "A")
					header += "1";
				else
					header += "0";
				found = 1;
				break;
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

        //now reply the client
        if (sendto(s, &full_message, recv_len, 0, (struct sockaddr*) &si_other, slen) == -1)
        {
            die("sendto()");
        }
    }
 
    close(s);
    return 0;
}
