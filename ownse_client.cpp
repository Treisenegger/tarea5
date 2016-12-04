/*
    Simple udp client
*/
#include<stdio.h> //printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<unistd.h> //close();
#include<arpa/inet.h>
#include<sys/socket.h>
#include<list>
#include<iostream>
#include<sstream>
#include<cstdlib>
 
#define SERVER "127.0.0.1"
#define BUFLEN 512  //Max length of buffer
#define PORT 1029   //The port on which to send data

using namespace std;

bool is_number(const std::string &str)
{
    return str.find_first_not_of("0123456789") == std::string::npos;
}

bool is_ip(const std::string &str)
{
    int fp = 0, lp, num;
    std::string subs, str2 = str;
    for (int i = 0; i < 3; i++)
    {
	lp = str2.find(".");
	if (lp > 3)
	    return false;
	subs = str2.substr(fp, lp-fp);
	if (!is_number(subs) || subs == "")
	    return false;
	num = atoi(subs.c_str());
	if (num > 255)
	    return false;
	str2 = str2.substr(lp+1);
    }
    subs = str2.substr(fp);
    if (!is_number(subs) || subs == "")
	return false;
    num = atoi(subs.c_str());
    if (num > 255)
	return false;
    return true;
}

std::string n_rand_bits(int n)
{
	std::string newString;
	for (int i = 0; i < n; i++)
		{
			newString += std::to_string(rand()%2);
		}
	return newString;
}

list<string> split(string s, string sep) {

	/*Splits a string on sep.*/

	list<string> l;

	size_t i = 0, pos = 0;

	while (pos != string::npos) {

		pos = s.find(sep, i);
		l.push_back(s.substr(i, pos - i));
		i = pos + 1;
	}

	return l;

}

void die(char *s)
{
    perror(s);
    exit(1);
}
 
int main(void)
{
    struct sockaddr_in si_other;
    int s, i, slen=sizeof(si_other);
    std::string buf;
    std::string message, header, full_message, reverse;
 
    if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }
 
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);
     
    if (inet_aton(SERVER , &si_other.sin_addr) == 0) 
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }
 
    while("Life is terrible")
    {
        printf("Enter message : ");
        fgets(message, BUFLEN, stdin);

		if (is_ip(message))
			reverse = "1";
		else reverse = "0";

	header = "";
	header = n_rand_bits(16) + "00" + reverse + "00000000000000000000000000001000000000000000000000000000000000000000000000000";

	full_message = header + message;

        //send the message
        if (sendto(s, full_message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen)==-1)
        {
            die("sendto()");
        }

	int received = 0;
	while(!received) {
		//receive a reply and print it
		//clear the buffer by filling null, it might have previously received data
		memset(&buf,'\0', BUFLEN);
		//try to receive some data, this is a blocking call
		if (recvfrom(s, &buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == -1)
		{
		    die("recvfrom()");
		}

		string r_mess = buf.substr(96);

		if (r_mess.substr(0, 4) != "NONE") {

			list<string> mess_list = split(r_mess, ",");
			list<string>::iterator it = mess_list.begin();
			advance(it, 2);
			if (*it == "A") {
				received = 1;
				it--;
				string value = *it;
				it--;
				string name = *it;
				cout << name + " has address" + value;
			}
			else {
				it--;
				string value = *it;
				it--;
				string name = *it;
				cout << name + " is an alias for " + value;
				header = n_rand_bits(16) + "00" + reverse + "00000000000000000000000000001000000000000000000000000000000000000000000000000";

				full_message = header + value;

				//send the message
				if (sendto(s, full_message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen)==-1)
				{
				    die("sendto()");
				}

			}
		}
		else {
			cout << "Host " + r_mess.substr(4) + " not found";
		}

		puts(buf);
	}
    }
 
    close(s);
    return 0;
}
