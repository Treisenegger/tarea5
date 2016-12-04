#include <list>
#include <string>
#include <iostream>
#include <fstream>
 
#define BUFLEN 512  //Max length of buffer
#define PORT 1029   //The port on which to listen for incoming data
#define FILEROUTE "./server.dns" //Route of file with dns information


using namespace std;

class association
{
	public:
		std::string name, value, type;
		
};


list<string> split(string s) {

	/*Splits a string on spaces.*/

	list<string> l;

	size_t i = 0, pos;

	while (pos != string::npos) {

		pos = s.find(" ", i);
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
    		l = split(line);

    		association ass;
    		ass.name = l.front();
    		l.pop_front();
    		ass.value = l.front();
    		l.pop_front();
    		ass.type = l.front();
    		l.pop_front();

    		ass_list.push_back(ass);
    	}
    	server_data.close();
  	}

  	else cout << "Unable to open file"; 

  	return ass_list;

}







int main(int argc, char const *argv[])
{
	list<association> l = parseFile(FILEROUTE);
	for (list<association>::iterator it = l.begin(); it != l.end(); it++)
    	cout << (*it).name << " " << (*it).value << " " << (*it).type << '\n';

	return 0;
}