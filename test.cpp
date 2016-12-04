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

int main() {
 std::string myString = "255.2..1";
 std::cout << is_ip(myString) << std::endl;
}
