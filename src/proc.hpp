#include <iostream>
#include <regex>
#include <dirent.h>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <chrono>
#include <cmath>
#include "str/fmt.cpp"
#include "proc.h"

Proc::Proc(){}

void Proc::usage(){
	std::cout << "proc: version " << VERSION <<
		"\n\nOutput process list\nPID PPID CMD\n\n"
		"Output process list with -stat\nPID PPID CPU MEM STARTTIME EXECTIME CMD\n\n"
		"Usage: proc [options]\n"
		"Options:\n"
		"\t-name <name>         -- Process name\n"
		"\t-grep <pattern>      -- Filter output grep alike\n"
		"\t-stat                -- Show CPU/mem\n"
		"\t-help\n" << '\n';
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

void Proc::stat(){
	_use_stat 		= true;
}

void Proc::run(){
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
	FILE* fd;
	const char* format_stat = "%*s %*s %*s %s %*s %*s %*s %*s %*s %*s %*s %*s %*s %s %s %s %s %s %*s %*s %*s %s %*s %s";
	char proc_ppid[6];			// 4
	char proc_utime[6];			// 14
	char proc_stime[6];			// 15
	char proc_cutime[6];		// 16
	char proc_cstime[6];		// 17
	char proc_priority[3];		// 18
	char proc_starttime[16];	// 22
	char proc_rss[10];			// 24
	
	char path_uptime[13];
	char sys_uptime[16];
	int CLK_TCK = 0;
	int PAGESIZE_KB = 0;
	double uptime = 0;
	
	int time = 0;
	double cputime;
	int seconds;
	
	if(_use_stat){
		CLK_TCK 	= sysconf(_SC_CLK_TCK);
		PAGESIZE_KB = sysconf(_SC_PAGESIZE) / 1024;
		
		//	Build path to /proc/uptime
		strcpy(path_uptime, _dir_proc);
		strcat(path_uptime, "/uptime");
		
		//	Open /proc/PID/stat
		fd = fopen(path_uptime, "r");
		if(!fd){
			throw std::runtime_error("Couldn't open directory "+std::string(path_uptime));
		}
		fscanf(fd, "%s", sys_uptime);
		fclose(fd);
		
		uptime 	= atof(sys_uptime);
		time 	= std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}
	
	for(int i = 0; (entry = readdir(dir)); i++){
		if(entry->d_type != DT_DIR || !val::is_digits(entry->d_name)){
			continue;
		}
		
		//	Build path to /proc/PID/cmdline
		strcpy(path_cmd, _dir_proc);
		strcat(path_cmd, entry->d_name);
		strcat(path_cmd, "/cmdline");
		
		//	Open /proc/PID/cmdline
		ifs.open(path_cmd, std::ifstream::in);
		if(!ifs){
			continue;
		}
		
		std::ostringstream oss;
		oss << ifs.rdbuf();
		cmd = oss.str();
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
		fd = fopen(path_stat, "r");
		if(!fd){
			continue;
		}
		
		if(_use_stat){
			fscanf(fd, format_stat, proc_ppid, proc_utime, proc_stime, proc_cutime, proc_cstime, proc_priority, proc_starttime, proc_rss);
			
			cputime = atoi(proc_utime) + atoi(proc_stime) + atoi(proc_cutime) + atoi(proc_cstime);
			seconds = uptime - (atoi(proc_starttime) / CLK_TCK);
			
			std::cout <<
				entry->d_name << " " <<
				proc_ppid << " " <<
				std::round((cputime / CLK_TCK / seconds) * 100 * 10.0) / 10.0 << "% " <<
				std::round(atof(proc_rss) * PAGESIZE_KB / 1024 * 10.0) / 10.0 << "M " <<
				time - seconds << " " <<
				seconds << " #" <<
				cmd << '\n';
		}
		else{
			fscanf(fd, "%*s %*s %*s %s", proc_ppid);
			
			std::cout <<
				entry->d_name << " " <<
				proc_ppid << " #" <<
				cmd << '\n';
		}
		
		fclose(fd);
	}
	
	closedir(dir);
}