/*
 *	Compile
 *	# g++ -Wall -O3 -std=c++17 proc.cpp -o ../proc
 */

#include <csignal>
#include <iostream>
#include <stdexcept>
#include "libstr/arg.h"
#include "liberr/err.h"
#include "proc.hpp"

int main(int argc, char* argv[]){
	//	Unsync C and C++ I/O (for better performance)
	std::ios::sync_with_stdio(false);
	
	//	Untie stdin from stdout (for better performance)
	std::cin.tie(nullptr);
	
	//	Trace segfaults
	struct sigaction sa{};
	sa.sa_handler = err::trace;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if(sigaction(SIGSEGV, &sa, nullptr) == -1){
		std::cerr << "Error: Failed to install SIGSEGV handler\n";
		
		return 1;
	}
	
	Proc a;
	
	try{
		//	Parse arguments
		for(int i = 1; i < argc; i++){
			char* value;
			
			if((value = arg::argument("name", argc, argv, i))){
				a.find_name(value);
			}
			else if((value = arg::argument("grep", argc, argv, i))){
				a.filter_cmd(value);
			}
			else if(arg::flag("stat", argv[i])){
				a.stat();
			}
			else if(arg::flag("help", argv[i])){
				a.usage();
				
				return 0;
			}
			else{
				throw std::invalid_argument(argv[i]);
			}
		}
		
		return a.run();
	}
	catch(const std::invalid_argument& e){
		std::cerr << "Error: Argument '" << e.what() << "' is invalid\n";
		a.usage();
		
		return 1;
	}
	catch(const std::exception& e){
		std::cerr << "Error: " << e.what() << '\n';
		
		return 1;
	}
}