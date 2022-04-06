#include <iostream>
#include <regex>
#include <dirent.h>
#include <fstream>
#include <sstream>
#include "str/fmt.cpp"
#include "proc.h"

Proc::Proc(){}

void Proc::usage(){
	std::cout << "proc: version " << VERSION <<
		"\n\nOutput process list\nPID PPID CMD\n\n"
		"Usage: proc [options]\n"
		"Options:\n"
		"\t-C <name>            -- Process name\n"
		"\t-grep <pattern>      -- Filter output grep alike\n" << '\n';
}

void Proc::find_name(const std::string& s){
	_use_name 		= true;
	_name 			= s;
}

void Proc::filter_cmd(const std::string& s){
	_use_filter 	= true;
	_filter 		= s;
	_filter_re 		= s;
}

std::string Proc::run(){
	DIR* dir = opendir(_dir_proc);
	if(dir == NULL){
		throw std::runtime_error("Couldn't open directory "+std::string(_dir_proc));
	}
	
	struct dirent* entry;
	
	char path_cmd[20];
	std::ifstream ifs;
	std::string cmd;
	std::string cmd_name;
	
	char path_stat[16];
	FILE* f_stat;
	char proc_pid[6];
	char proc_name[17];
	char proc_state[2];
	char proc_ppid[6];
	
	for(int i = 0; entry = readdir(dir); i++){
		if(entry->d_type == DT_DIR && val::is_digits(entry->d_name)){
			//	Build path to /proc/PID/cmdline
			strcpy(path_cmd, _dir_proc);
			strcat(path_cmd, entry->d_name);
			strcat(path_cmd, "/cmdline");
			
			//	Open /proc/PID/cmdline
			ifs.open(path_cmd, std::ifstream::in);
			if(!ifs){
				continue;
			}
			
			std::stringstream ss;
			ss << ifs.rdbuf();
			cmd = ss.str();
			ifs.close();
			
			//	Replace NULL bytes with space
			fmt::replace('\0', ' ', cmd);
			
			if(_use_name){
				//	Get name of cmd
				cmd_name = fmt::basename(cmd.substr(0, cmd.find(' ')));
				
				if(_name != cmd_name){
					continue;
				}
			}
			
			//	Filter
			if(_use_filter && !std::regex_search(cmd, _filter_rem, _filter_re)){
				continue;
			}
			
			//	Build path to /proc/PID/stat
			strcpy(path_stat, _dir_proc);
			strcat(path_stat, entry->d_name);
			strcat(path_stat, "/stat");
			
			//	Open /proc/PID/stat
			f_stat = fopen(path_stat, "r");
			if(!f_stat){
				continue;
			}
			fscanf(f_stat, "%s", proc_pid);
			fscanf(f_stat, "%s", proc_name);
			fscanf(f_stat, "%s", proc_state);
			fscanf(f_stat, "%s", proc_ppid);
			fclose(f_stat);
			
			std::cout << proc_pid << " " << proc_ppid << " " << cmd << '\n';
		}
	}
	closedir(dir);
	
	return "";
}