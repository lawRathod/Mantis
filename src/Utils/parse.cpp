#include"parse.h"

int main() {

	raster *core = new raster("CupriteAVIRISSubset.hdr", "CupriteAVIRISSubset.dat");
	auto *data = core->read_bin();
	
	core->print_data_sample(data, 1, 1);

	std::cout<<std::endl;

	return 0;

}

