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


int main() {
 std::string myString = "1542Hello6156512aASdasd6a121156as1d61as1d1a6s16d5as";
 for (int i = 0; i < myString.size(); i++) {
 if (char(myString[i]).isdigit()) {
 std::cout << myString[i];
}
}
}
