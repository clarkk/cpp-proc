#include <string.h>

namespace val {
	bool is_digits(const char* c){
		for( ; *c; c++){
			if(*c < '0' || *c > '9'){
				return false;
			}
		}
		
		return true;
	}
	
	bool starts_with(const char* a, const char* b){
		return strncmp(a, b, strlen(b)) == 0;
	}
}