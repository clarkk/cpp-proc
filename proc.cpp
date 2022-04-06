/*
 *	Compile
 *	# g++ -O2 -std=c++17 proc.cpp -o proc
 */

#include "proc.hpp"
#include <string.h>
//#include <dirent.h>
//#include <fstream>
//#include <sstream>
//#include <algorithm>
//#include <regex>

/*int is_numeric(const char* c){
	for( ; *c; c++){
		if(*c < '0' || *c > '9'){
			return 0;
		}
	}
	
	return 1;
}*/

void ps(){
	/*bool use_name 			= false;
	std::string arg_name;
	
	bool use_filter 		= false;
	std::string arg_filter;
	
	std::smatch rem;
	std::regex re;
	
	//	Match cmd name
	if(argc > 1){
		use_name 			= true;
		arg_name 			= std::string(argv[1]);
	}
	
	//	Match cmd filter
	if(argc > 2){
		use_filter 			= true;
		arg_filter 			= std::string(argv[2]);
		re 					= arg_filter;
	}
	
	const char* dir_proc 	= "/proc/";
	DIR* dir 				= opendir(dir_proc);
	
	if(dir == NULL){
		std::cerr << "Error: Couldn't open directory" << dir_proc << std::endl;
		
		return 1;
	}
	
	struct dirent* entry;
	
	char path_cmd[20];
	char path_stat[16];
	
	std::string cmd_name;
	
	char proc_pid[6];
	char proc_name[17];
	char proc_state[2];
	char proc_ppid[6];
	
	for(int i = 0; entry = readdir(dir); i++){
		if(entry->d_type == DT_DIR && is_numeric(entry->d_name)){
			//	Build path to /proc/PID/cmdline
			strcpy(path_cmd, dir_proc);
			strcat(path_cmd, entry->d_name);
			strcat(path_cmd, "/cmdline");
			
			//	Open /proc/PID/cmdline
			std::ifstream ifs;
			ifs.open(path_cmd, std::ifstream::in);
			if(!ifs){
				continue;
			}
			std::stringstream ss;
			ss << ifs.rdbuf();
			std::string cmd = ss.str();
			ifs.close();
			
			//	Replace NULL bytes with space
			std::replace(cmd.begin(), cmd.end(), '\0', ' ');
			
			if(use_name){
				//	Get name of cmd
				cmd_name = cmd.substr(0, cmd.find(" "));	// Get cmd before first space
				cmd_name = cmd_name.substr(cmd_name.find_last_of("/\\") + 1);	// Get basename of cmd path
				
				if(arg_name != cmd_name){
					continue;
				}
				
				//	Filter
				if(use_filter && !std::regex_search(cmd, rem, re)){
					continue;
				}
			}
			
			//	Build path to /proc/PID/stat
			strcpy(path_stat, dir_proc);
			strcat(path_stat, entry->d_name);
			strcat(path_stat, "/stat");
			
			//	Open /proc/PID/stat
			FILE* f_stat = fopen(path_stat, "r");
			if(!f_stat){
				continue;
			}
			fscanf(f_stat, "%s", proc_pid);
			fscanf(f_stat, "%s", proc_name);
			fscanf(f_stat, "%s", proc_state);
			fscanf(f_stat, "%s", proc_ppid);
			fclose(f_stat);
			
			std::cout << proc_pid << " " << proc_ppid << " " << cmd << std::endl;
		}
	}
	closedir(dir);*/
}

bool StartsWith(const char* a, const char* b){
	return strncmp(a, b, strlen(b)) == 0;
}

int main(int argc, char* argv[]){
	Proc a;
	
	std::string arg;
	int v;
	
	try{
		//	Parse arguments
		for(int i = 1; i < argc; i++){
			arg = std::string(argv[i]);
			v = i+1;
			
			if(arg == "-C"){
				if(argc <= v || StartsWith(argv[v], "-")){
					throw std::invalid_argument("Argument '"+arg+"' excepted a value");
				}
				
				a.find_name(argv[++i]);
			}
			else if(arg == "-grep"){
				if(argc <= v || StartsWith(argv[v], "-")){
					throw std::invalid_argument("Argument '"+arg+"' excepted a value");
				}
				
				a.filter_cmd(argv[++i]);
			}
			else{
				throw std::invalid_argument("Argument '"+arg+"' is invalid");
			}
		}
		
		//std::cout << a.run() << std::endl;
	}
	catch(const std::invalid_argument& e){
		std::cerr << "Error: " << e.what() << '\n';
		a.usage();
		
		return 1;
	}
	
	return 0;
}