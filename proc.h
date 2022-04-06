class Proc{
public:
	Proc();
	
	const std::string VERSION			= "0.1";
	void usage							();
	void find_name						(const std::string& s);
	void filter_cmd						(const std::string& s);
	//void set_level						(const std::string& s);
	//void set_dpi 						(int d);
	//void set_verbose					(bool d);
	//std::string run						();

private:
	std::string _name 					= "";
	std::string _filter 				= "";
	/*tesseract::PageIteratorLevel level;
	bool is_verbose						= false;
	int dpi 							= DPI;
	void error							(const std::string& s);*/
};