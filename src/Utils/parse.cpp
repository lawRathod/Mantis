#include"parse.h"
#include <iterator>
#include<regex>

namespace Utils {

	raster::raster(std::string hdrfile, std::string datfile){
		this->hdrfile = hdrfile;
		this->datfile = datfile;
		read_metadata();
	}

	void raster::read_metadata(){

		std::ifstream header;
		header.open(hdrfile);
		std::string line;

		while(header){
			getline(header, line);
			int equal_pos = line.find("=");
			if(equal_pos > -1){
				std::string key =line.substr(0, line.find("=")-1);
				key = std::regex_replace(key.c_str(), std::regex("^ +| +$|( ) +"), "$1");
				std::string value = line.substr(equal_pos + 2);
				value = std::regex_replace(value.c_str(), std::regex("^ +| +$|( ) +"), "$1");
				metadata.insert(std::pair<std::string, std::string>(key, value));
			}
		}

		header.close();
	}

	template<typename T>
	void* raster::read_bin_internal(){
		std::stringstream *ss;
		ss = new std::stringstream(metadata.at("bands"));
		int bands;
		*ss >> bands;
		ss = new std::stringstream(metadata.at("samples"));
		int samples;
		*ss >> samples;
		ss = new std::stringstream(metadata.at("lines"));
		int lines;
		*ss >> lines;

		p = new T [bands * lines * samples];
		std::ifstream bin(datfile, std::ios::out | std::ios::binary);

		int count = 0;
		for(int i=0;i<bands; i++){
			for(int j=0;j<samples; j++){
				for(int k=0;k<lines;k++){
					bin.read((char *) &*(static_cast<T*>(p) + i*samples*lines + j*lines + k), sizeof(static_cast<T*>(p)));
					count++;
				}
			}
		}

		bin.close();

		return p;

	}

	void* raster::read_bin(){
		if(metadata.at("data type").compare("2\r") == 0){
			return read_bin_internal<int16_t>();
		}
		if(metadata.at("data type").compare("1\r") == 0){
			return read_bin_internal<uint8_t>();
		}
		return NULL;
	}


	std::string raster::get_metadata(std::string key){
		return metadata.at(key);
	}

}


