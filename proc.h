class Proc{
	public:
		Proc();
		
		const std::string VERSION			= "0.1";
		void usage							();
		void set_name						(const std::string& s);
		//void set_level						(const std::string& s);
		//void set_dpi 						(int d);
		//void set_verbose					(bool d);
		//std::string run						();
	
	private:
		std::string find_name 				= "";
		/*tesseract::PageIteratorLevel level;
		bool is_verbose						= false;
		int dpi 							= DPI;
		void error							(const std::string& s);*/
};