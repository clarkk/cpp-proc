#include <iostream>
#include "proc.h"

Proc::Proc(){}

void Proc::usage(){
	std::cout<< "proc: version " << VERSION <<
		"\n\nUsage: proc [options]\n"
		"Options:\n"
		"\t-C <name>            -- Process name\n"
		"\t-grep <pattern>      -- Filter output grep alike\n" << std::endl;
}

void Proc::find_name(const std::string& s){
	_name = s;
}

void Proc::filter_cmd(const std::string& s){
	_filter = s;
}

/*void Txtocr::set_level(const std::string& s){
	if(s == ""){
		level = tesseract::RIL_WORD;
	}
	else if(s == "word"){
		level = tesseract::RIL_WORD;
	}
	else if(s == "symbol"){
		level = tesseract::RIL_SYMBOL;
	}
	else{
		error("Invalid iterator level");
	}
}

void Txtocr::set_dpi(int d){
	dpi = d;
}

void Txtocr::set_verbose(bool v){
	is_verbose = v;
}

void Txtocr::error(const std::string& s){
	throw std::runtime_error(s);
}

std::string Txtocr::run(){
	//	Return error if input file is not defined
	if(input == ""){
		error("Input file not defined");
	}
	//	Return error if input file is not TIFF
	else{
		std::string input_lc = input;
		boost::to_lower(input_lc);
		if(input_lc.substr(input_lc.find_last_of(".") + 1) != "tif"){
			error("Input file must be TIFF");
		}
	}
	
	auto start = std::chrono::high_resolution_clock::now();
	
	//	Open input image with leptonica library
	Pix* image = pixRead((input).c_str());
	
	if(!image){
		error("Could not open image");
	}
	
	boost::property_tree::ptree root;
	boost::property_tree::ptree children;
	
	root.put("height", pixGetHeight(image));
	root.put("width", pixGetWidth(image));
	
	//	Initialize tesseract
	tesseract::TessBaseAPI* api = new tesseract::TessBaseAPI();
	//if(api->Init(NULL, "dan+eng", tesseract::OEM_LSTM_ONLY)){
	if(api->Init("/usr/share/tesseract-ocr/5/tessdata/best", "dan+eng")){
		error("Could not initialize tesseract");
	}
	
	char dpi_string[255];
	snprintf(dpi_string, 254, "%d", dpi);
	api->SetVariable("user_defined_dpi", dpi_string);
	
	api->SetImage(image);
	api->Recognize(0);
	
	tesseract::ResultIterator* ri = api->GetIterator();
	
	if(ri != 0){
		do{
			boost::property_tree::ptree child;
			
			const char* seg = ri->GetUTF8Text(level);
			
			if(seg && seg[0]){
				int x1, y1, x2, y2, height, width;
				ri->BoundingBox(level, &x1, &y1, &x2, &y2);
				height 	= y2 - y1;
				width 	= x2 - x1;
				
				if(is_verbose){
					printf("seg: '%s'; BoundingBox: %d,%d,%d,%d;\n", seg, x1, y1, x2, y2);
				}
				
				child.put("top",	y1);
				child.put("bottom",	y1 + height);
				child.put("left",	x1);
				child.put("right",	x1 + width);
				child.put("width",	width);
				child.put("height",	height);
				child.put("value",	seg);
				child.put("conf",	roundf(ri->Confidence(level) * 100) / 100);
				
				children.push_back(std::make_pair("", child));
			}
			
			delete[] seg;
		}
		while(ri->Next(level));
		
		root.add_child("elms", children);
	}
	
	// Destroy used object and release memory
	api->End();
	pixDestroy(&image);
	
	auto elapsed = std::chrono::high_resolution_clock::now() - start;
	root.put("exec_time", ((float)std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count() / 1000) / 1000);
	
	std::ostringstream oss;
	write_json(oss, root, false);
	
	return oss.str();
}*/