#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <err.h>

// thread libs and for each
#include <limits.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <algorithm>
#include <functional>

using namespace std;

const void exec(const string &cmd, const size_t size, vector<string> &results);
size_t get_cores();

mutex mtx;

int main(int argc, char const *argv[]){

string command = "";
string ssize;
size_t size; // this size is used to allocate the buffer that stores the output of the scripts
vector<thread> threads;
vector<string> results;
size_t i;

const size_t numcpu = get_cores();

	if(argc <= 1)
		err(0, "Must pass a file path");

ifstream file(argv[1]);
	
	if(!file)
		err(0, "invalid file");

	getline(file, ssize);

	size = atoi(ssize.c_str());

	// void (*ex)(const string &cmd, const size_t size, vector<string> &results);
	// ex = &exec;

	// this loop *should* go through the file, taking in 'numcpu' lines at a time, and creating 'numcpu' threads
	while (file){
		for(i = 0; i < numcpu && (file); i++){
			threads.push_back(thread(exec, command, size,  std::ref(results)));
			getline(file, command);
		}
		for_each(threads.begin(), threads.end(), mem_fn(&std::thread::join));
		threads.clear();
	}

	// cout results
	for (vector<string>::iterator it = results.begin() ; it != results.end(); ++it)
 			cout << *it << endl;

	return 0;
}

size_t get_cores(){

size_t tmpCPU;
size_t MAX_CPU;
std::vector<string> results;

	ifstream file("/proc/sys/kernel/threads-max");
	string tmp;
	getline(file, tmp);
	MAX_CPU = atoi(tmp.c_str());
	
	if ((tmpCPU = sysconf( _SC_NPROCESSORS_ONLN )) < 1 || tmpCPU > MAX_CPU)
		tmpCPU = 2; // If we get some weird response, assume that the system is at least a dual core.

	return tmpCPU;
}

const void exec(const string &command, const size_t size, vector<string> &results){

	char * buffer = new char[size];
    string result = "";

    FILE * pipe = popen(command.c_str(), "r");

    if (!pipe) 
    	err(0, "Pipe is null");

    while(!feof(pipe))
    	if(fgets(buffer, size, pipe) != NULL)
    		result += buffer;

    mtx.lock();
    results.push_back(result);
    mtx.unlock();
    
    pclose(pipe);
    //return result;
}