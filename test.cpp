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
#include<cstdlib>

std::string n_rand_bits(int n)
{
	std::string newString;
	for (int i = 0; i < n; i++)
		{
			int j = rand()%2;
			newString += std::to_string(j);
		}
	return newString;
}

int main() {
	std::cout << n_rand_bits(16);
}
