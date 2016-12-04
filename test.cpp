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


int main()
{
std::string myString = "Hello World!";
std::string myOtherString = "!!!!";
std::string myThirdString = myString + "," + myOtherString;
std::cout << myThirdString;
for (int i = 0; i < myThirdString.size(); i++)
	std::cout << std::bitset<8> b(myThirdString[i]);
}

