#include<iostream>
#include<string>
#include<glob.h>
#include<fstream>
#include<vector>
#include<sstream>


class raster{
	void getfiles(){
		std::cout<<".hdr file (full path)\n";
		std::cin>>hdrfile;
		std::cout<<".dat/.envi file (full path)\n";
		std::cin>>datfile;
	}

	public: 
	int samples, lines, bands, datatype, header_offset, byteorder;
	std::string datfile, hdrfile;
	raster(){
		getfiles();
	}

	raster(std::string hdrfile, std::string datfile){
		this->hdrfile = hdrfile;
		this->datfile = datfile;
		get_metadata();
	}

	void get_metadata(){

		std::ifstream header;
		header.open(hdrfile);
		std::string line;

		int i = 12;
		while(i--){
			getline(header, line);

			if(line.find("samples")!= -1){
				std::string s = line.substr(line.find("=")+1);
				std::stringstream ss(s);
				ss >> samples;
			}
			if(line.find("lines")!= -1){
				std::string s = line.substr(line.find("=")+1);
				std::stringstream ss(s);
				ss >> lines;
			}
			if(line.find("bands")!= -1){
				std::string s = line.substr(line.find("=")+1);
				std::stringstream ss(s);
				ss >> bands;
			}
			if(line.find("data type")!= -1){
				std::string s = line.substr(line.find("=")+1);
				std::stringstream ss(s);
				ss >> datatype;
			}
			if(line.find("header_offset")!= -1){
				std::string s = line.substr(line.find("=")+1);
				std::stringstream ss(s);
				ss >> header_offset;
			}
			if(line.find("byte order")!= -1){
				std::string s = line.substr(line.find("=")+1);
				std::stringstream ss(s);
				ss >> byteorder;
			}
		}
		header.close();
	}

	template<typename T>
	auto* read_binary_internal(T *p, T q){
		p = new T [bands * samples * lines];

		std::ifstream bin(datfile, std::ios::out | std::ios::binary);

		int count = 0;
		for(int i=0;i<bands; i++){
			for(int j=0;j<samples; j++){
				for(int k=0;k<lines;k++){
					bin.read((char *) &q, sizeof(q));
					*(p + i*samples*lines + j*lines + k) = q;
					count++;
				}
			}
		}
		std::cout<<count<<"\n";

		bin.close();

		return p;

	}

	auto* read_bin(){
		if(datatype == 2){
			int16_t *p;
			int16_t q;
			return read_binary_internal<int16_t>(p, q);
		} else if( datatype == 1 ){
			uint8_t *p;
			uint8_t q;
			return read_binary_internal<uint8_t>(p, q);
		}

	}

	static void print_data_sample(int16_t* data, int band_from_hdr, int sample_num, int total_lines){
		for(int i=0;i<band_from_hdr; i++){
			for(int j=0;j<sample_num; j++){
				for(int k=0;k<total_lines;k++){
					std::cout<<*(data + i*sample_num*total_lines + j*total_lines + k)<<" ";
				}
				std::cout<<std::endl;
			}
			std::cout<<std::endl;
		}
	}

};


int main() {

	raster *core = new raster("CupriteAVIRISSubset.hdr", "CupriteAVIRISSubset.dat");
	auto *data = core->read_bin();
	
	raster::print_data_sample(data, 10,10, core->lines);

	std::cout<<std::endl;

	return 0;

}

