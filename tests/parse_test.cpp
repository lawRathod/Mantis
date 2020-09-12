#include<iostream>
#include"../src/Utils/parse.h"
namespace Utils {

	template<typename T>
	void raster::print_data_sample_internal(void* data, int band_from_hdr, int sample_num){
		std::stringstream ss(metadata.at("lines  "));
		int lines;
		ss>>lines;
		for(int i=0;i<band_from_hdr; i++){
			for(int j=0;j<sample_num; j++){
				for(int k=0;k<lines;k++){
					std::cout<<*(static_cast<T*>(data) + i*sample_num*lines + j*lines + k)<<" ";
				}
				std::cout<<std::endl;
			}
			std::cout<<std::endl;
		}
	}
	void raster::print_data_sample(void* data, int band_from_hdr, int sample_num){
		if(metadata.at("data type") == "2\r"){
			print_data_sample_internal<int16_t>(data, band_from_hdr, sample_num);
		}else if (metadata.at("data type") == "1\r"){
			print_data_sample_internal<uint8_t>(data, band_from_hdr, sample_num);
		}
	}
}

