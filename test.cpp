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
