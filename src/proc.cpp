/*
 *	Compile
 *	# g++ -Wall -O3 -std=c++17 proc.cpp -o ../proc
 */

#include "libstr/arg.h"
#include "proc.hpp"

int main(int argc, char* argv[]){
	//	Unsync C and C++ I/O (for better performance)
	std::ios::sync_with_stdio(false);
	
	//	Untie stdin from stdout (for better performance)
	std::cin.tie(NULL);
	
	Proc a;
	
	try{
		//	Parse arguments
		for(int i = 1; i < argc; i++){
			if(char* value = arg::argument("name", argc, argv, i)){
				a.find_name(value);
			}
			else if(char* value = arg::argument("grep", argc, argv, i)){
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
				throw std::invalid_argument("Argument '"+std::string(argv[i])+"' is invalid");
			}
		}
		
		return a.run();
	}
	catch(const std::invalid_argument& e){
		std::cerr << "Error: " << e.what() << "\n\n";
		a.usage();
		
		return 1;
	}
	catch(const std::runtime_error& e){
		std::cerr << "Error: " << e.what() << '\n';
		
		return 1;
	}
}