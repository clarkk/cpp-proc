class Proc{
public:
	Proc();
	
	const std::string VERSION			= "0.1";
	void usage							();
	
	void find_name						(const std::string& s);
	void filter_cmd						(const std::string& s);
	
	std::string run						();

private:
	bool _use_name 						= false;
	std::string _name 					= "";
	
	bool _use_filter 					= false;
	std::string _filter 				= "";
	
	std::smatch _filter_rem;
	std::regex _filter_re;
	
	const char* _dir_proc 				= "/proc/";
};