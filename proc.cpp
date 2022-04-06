/*
 *	Compile
 *	# g++ -O2 -std=c++17 proc.cpp -o proc
 */

//#include <algorithm>
#include "str/val.cpp"
#include "proc.hpp"

int main(int argc, char* argv[]){
	Proc a;
	
	std::string arg;
	int v;
	
	try{
		//	Parse arguments
		for(int i = 1; i < argc; i++){
			arg 	= std::string(argv[i]);
			v 		= i + 1;
			
			if(arg == "-C"){
				if(argc <= v || val::starts_with(argv[v], "-")){
					throw std::invalid_argument("Argument '"+arg+"' excepted a value");
				}
				
				a.find_name(argv[++i]);
			}
			else if(arg == "-grep"){
				if(argc <= v || val::starts_with(argv[v], "-")){
					throw std::invalid_argument("Argument '"+arg+"' excepted a value");
				}
				
				a.filter_cmd(argv[++i]);
			}
			else{
				throw std::invalid_argument("Argument '"+arg+"' is invalid");
			}
		}
		
		std::cout << a.run() << '\n';
		
		return 0;
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