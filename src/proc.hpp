#include <iostream>
#include <regex>
#include <dirent.h>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <ctime>
#include <cmath>
#include "libstr/fmt.h"
#include "libstr/val.h"
#include "proc.h"

Proc::Proc(){}

void Proc::usage(){
	std::cout << "proc: " << VERSION << "\n\n"
		"Output process list\nPID PPID CMD\n\n"
		"Output process list with -stat\nPID PPID CPU MEM STARTTIME EXECTIME CMD\n\n"
		"Usage: proc [options]\n"
		"Options:\n"
		"\t-name <name>         -- Process name\n"
		"\t-grep <pattern>      -- Filter output grep alike\n"
		"\t-stat                -- Show CPU/mem\n"
		"\t-help\n"
		<< '\n';
}

void Proc::find_name(const std::string& s){
	_use_name 		= true;
	_name 			= s;
}

void Proc::filter_cmd(const std::string& s){
	_use_filter 	= true;
	_filter 		= s;
	
	try{
		_filter_re = std::regex(s, std::regex_constants::optimize);
	}
	catch(const std::regex_error&){
		throw std::invalid_argument("Invalid regex pattern '"+s+"'");
	}
}

void Proc::stat(){
	_use_stat 		= true;
}

int Proc::run(){
	DIR* dir = opendir(_dir_proc);
	if(dir == NULL){
		throw std::runtime_error("Couldn't open directory "+std::string(_dir_proc));
	}
	
	struct dirent* entry;
	
	std::string
		cmd,
		cmd_name,
		stat_line;
	
	std::string path_buf;
	path_buf.reserve(64);
	
	std::ifstream ifs;
	
	double
		CLK_TCK 		= 0,
		PAGESIZE_KB 	= 0,
		uptime 			= 0,
		cputime,
		seconds;
	
	struct proc_stat {
		long ppid;
		long utime;
		long stime;
		long cutime;
		long cstime;
		long starttime;
		long rss;
	};
	
	std::time_t time = 0;
	
	if(_use_stat){
		CLK_TCK 	= sysconf(_SC_CLK_TCK);
		PAGESIZE_KB = sysconf(_SC_PAGESIZE) / 1024.0;
		
		path_buf = _dir_proc;
		path_buf += "uptime";
		
		//	Open /proc/uptime
		ifs.open(path_buf);
		if(!ifs){
			closedir(dir);
			throw std::runtime_error("Couldn't open file "+path_buf);
		}
		
		ifs >> uptime;
		
		ifs.close();
		ifs.clear();
		
		time = std::time(nullptr);
	}
	
	while((entry = readdir(dir))){
		//	Skip non-process directories
		if(entry->d_type != DT_DIR || !val::is_digits(entry->d_name)){
			continue;
		}
		
		path_buf = _dir_proc;
		path_buf += entry->d_name;
		path_buf += "/cmdline";
		
		//	Open /proc/PID/cmdline
		ifs.open(path_buf, std::ifstream::in);
		if(!ifs){
			ifs.clear();
			continue;
		}
		
		//	Read entire cmdline
		cmd.assign(
			std::istreambuf_iterator<char>(ifs),
			std::istreambuf_iterator<char>()
		);
		
		ifs.close();
		ifs.clear();
		
		//	Replace NULL separators with spaces
		fmt::replace('\0', ' ', cmd);
		
		//	Skip empty cmdlines
		if(cmd.empty()){
			continue;
		}
		
		if(_use_name){
			//	Get name of cmd
			cmd_name = fmt::basename(cmd.substr(0, cmd.find(' ')));
			
			if(_name != cmd_name){
				continue;
			}
		}
		
		//	Filter
		if(_use_filter){
			if(!std::regex_search(cmd, _filter_re)){
				continue;
			}
		}
		
		path_buf = _dir_proc;
		path_buf += entry->d_name;
		path_buf += "/stat";
		
		//	Open /proc/PID/stat
		ifs.open(path_buf);
		if(!ifs){
			ifs.clear();
			continue;
		}
		
		std::getline(ifs, stat_line);
		
		ifs.close();
		ifs.clear();
		
		/*
			/proc/[pid]/stat format:
			pid (process name) state ppid ...
		*/
		size_t pos = stat_line.rfind(')');
		
		if(pos == std::string::npos){
			continue;
		}
		
		//	Data after process name
		std::istringstream iss(stat_line.substr(pos + 2));
		
		char state;
		proc_stat st;
		long ignore;
		
		//	Field 3
		iss >> state;
		
		//	Field 4
		iss >> st.ppid;
		
		//	Skip fields 5-13
		for(int i = 0; i < 9; i++){
			iss >> ignore;
		}
		
		//	Fields 14-17
		iss >> st.utime;
		iss >> st.stime;
		iss >> st.cutime;
		iss >> st.cstime;
		
		//	Skip fields 18-21
		for(int i = 0; i < 4; i++){
			iss >> ignore;
		}
		
		//	Field 22
		iss >> st.starttime;
		
		//	Skip field 23
		iss >> ignore;
		
		//	Field 24
		iss >> st.rss;
		
		//	Validate parsing
		if(iss.fail()){
			continue;
		}
		
		if(_use_stat){
			seconds = uptime - (st.starttime / CLK_TCK);
			
			//	Avoid divide-by-zero
			if(seconds <= 0){
				seconds = 0.1;
			}
			
			cputime = st.utime + st.stime + st.cutime + st.cstime;
			
			long seconds_display = std::lround(seconds);
			
			std::cout <<
				entry->d_name << " " <<
				st.ppid << " " <<
				std::round((cputime / CLK_TCK / seconds) * 100 * 10.0) / 10.0 << "% " <<
				std::round(st.rss * PAGESIZE_KB / 1024.0 * 10.0) / 10.0 << "M " <<
				std::time_t(time - seconds_display) << " " <<
				seconds_display << " #" <<
				cmd << '\n';
		}
		else{
			std::cout <<
				entry->d_name << " " <<
				st.ppid << " #" <<
				cmd << '\n';
		}
	}
	
	closedir(dir);
	
	return 0;
}