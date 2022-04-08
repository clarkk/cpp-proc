namespace fmt {
	std::string basename(const std::string& s){
		return s.substr(s.find_last_of("/\\") + 1);
	}
	
	void replace(const char f, const char r, std::string& s){
		std::replace(s.begin(), s.end(), f, r);
	}
}