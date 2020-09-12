#include<iostream>
#include<string>
#include<fstream>
#include<unordered_map>
#include<sstream>


class raster{
	void * p;

	void getfiles(){
		std::cout<<".hdr file (full path)\n";
		std::cin>>hdrfile;
		std::cout<<".dat/.envi file (full path)\n";
		std::cin>>datfile;
	}

	public: 
	std::unordered_map<std::string, int> metadata;
	std::string datfile, hdrfile;
	raster(){
		getfiles();
	}

	raster(std::string hdrfile, std::string datfile){
		this->hdrfile = hdrfile;
		this->datfile = datfile;
		read_metadata();
	}

	void read_metadata(){

		std::ifstream header;
		header.open(hdrfile);
		std::string line;

		int i = 12;
		while(i--){
			getline(header, line);

			if(line.find("samples")!= -1){
				std::string s = line.substr(line.find("=")+1);
				std::stringstream ss(s);
				int t;
				ss>>t;
				metadata["samples"] = t;
			}
			if(line.find("lines")!= -1){
				std::string s = line.substr(line.find("=")+1);
				std::stringstream ss(s);
				int t;
				ss>>t;
				metadata.insert(std::pair<std::string, int>("lines", t));
				metadata["lines"] = t;
			}
			if(line.find("bands")!= -1){
				std::string s = line.substr(line.find("=")+1);
				std::stringstream ss(s);
				int t;
				ss>>t;
				metadata.insert(std::pair<std::string, int>("bands", t));
				metadata["bands"] = t;
			}
			if(line.find("data type")!= -1){
				std::string s = line.substr(line.find("=")+1);
				std::stringstream ss(s);
				int t;
				ss>>t;
				metadata["datatype"] = t;
			}
			if(line.find("header_offset")!= -1){
				std::string s = line.substr(line.find("=")+1);
				std::stringstream ss(s);
				int t;
				ss>>t;
				metadata["header_offset"] = t;
			;
			metadata.insert(std::pair<std::string, int>("header_offset",t));
			}
			if(line.find("byte order")!= -1){
				std::string s = line.substr(line.find("=")+1);
				std::stringstream ss(s);
				int t;
				ss >> t;
				metadata["byte_order"] = t;
			}
		}
		header.close();
	}

	template<typename T>
	void* read_bin_internal(){
		p = new T [metadata.at("bands") * metadata.at("samples") * metadata.at("lines")];
		std::ifstream bin(datfile, std::ios::out | std::ios::binary);

		int count = 0;
		for(int i=0;i<metadata.at("bands"); i++){
			for(int j=0;j<metadata.at("samples"); j++){
				for(int k=0;k<metadata.at("lines");k++){
					bin.read((char *) &*(static_cast<T*>(p) + i*metadata.at("samples")*metadata.at("lines") + j*metadata.at("lines") + k) , sizeof(static_cast<T*>(p)));
					count++;
				}
			}
		}
		std::cout<<count<<"\n";

		bin.close();

		return p;

	}

	void* read_bin(){
		if(metadata.at("datatype") == 2){
			return read_bin_internal<int16_t>();
		}
		if(metadata.at("datatype") ==1 ){
			return read_bin_internal<uint8_t>();
		}


		return NULL;
	}


	template<typename T>
	void print_data_sample_internal(void* data, int band_from_hdr, int sample_num){
		for(int i=0;i<band_from_hdr; i++){
			for(int j=0;j<sample_num; j++){
				for(int k=0;k<metadata.at("lines");k++){
					std::cout<<*(static_cast<T*>(data) + i*sample_num*metadata.at("lines") + j*metadata.at("lines") + k)<<" ";
				}
				std::cout<<std::endl;
			}
			std::cout<<std::endl;
		}
	}

	void print_data_sample(void* data, int band_from_hdr, int sample_num){
		if(metadata.at("datatype")==2){
			print_data_sample_internal<int16_t>(data, band_from_hdr, sample_num);
		}else if (metadata.at("datatype")==1){
			print_data_sample_internal<uint8_t>(data, band_from_hdr, sample_num);
		}
	}

	std::unordered_map<std::string, int> get_metadata(){
		return metadata;
	}

};


