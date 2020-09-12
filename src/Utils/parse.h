#include<string>
#include<fstream>
#include<unordered_map>
#include<sstream>

namespace Utils {
	class raster{
		public:
			std::unordered_map<std::string, int> metadata;
			std::string datfile, hdrfile;

			raster(std::string, std::string);
			void read_metadata();
			template<typename T> void* read_bin_internal();
			void* read_bin();
			template<typename T> void print_data_sample_internal(void*, int, int);
			void print_data_sample(void*, int, int);
			int raster::get_metadata(std::string);

		private:
			void * p;
			void getfiles();
	};
}

