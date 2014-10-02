#include <string>
#include <iostream>
#include <stdio.h>
#include <err.h>
#include <vector>
#include <fstream>

using namespace std;

string exec(const string &cmd, const size_t size);

int main(int argc, char const *argv[]){

string command = "";
string ssize;
size_t size; // this size is used to allocate the buffer that stores the output of the scripts

int i = 0;
	if(argc < 1)
		err(0, "Must pass a file path");

ifstream file(argv[1]);
	
	if(!file)
		err(0, "invalid file");

	getline(file, ssize);

	size = atoi(ssize.c_str());
	
	while (file){
		cout << exec(command, size) << endl; // this line should come after, cout prevents segfault *for now*. figure this out later.
		getline(file, command);
		
	}

	return 0;
}

string exec(const string &command, const size_t size){

	char * buffer = new char[size];
    string result = "";

    FILE * pipe = popen(command.c_str(), "r");

    if (!pipe) 
    	err(0, "Pipe is null");

    while(!feof(pipe))
    	if(fgets(buffer, size, pipe) != NULL)
    		result += buffer;

    pclose(pipe);
    return result;
}