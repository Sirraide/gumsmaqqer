#include "image.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "io.h"

#include <cstdlib>
#include <stb/stb_image_write.h>

using namespace std;
image::image(const std::string& filename) {
	io::file<char, io::filemode::rb>(filename, io::perror_and_exit);
	if (data = stbi_load(filename.c_str(), &width, &height, &channels, 0), !data)
		throw std::runtime_error("Could not load image from file '" + filename + "'");
	size = width * height * channels;
}
image::image(int _width, int _height, int _channels) : width(_width), height(_height), channels(_channels) {
	size  = width * height * channels;
	alloc = true;
	data  = static_cast<data_t*>(calloc(size, 1));
}
image::~image() {
	if (alloc) free(data);
	else
		stbi_image_free(data);
}
void image::save(const string& filename) const {
	stbi_write_png(filename.c_str(), width, height, channels, data, width * channels);
}
pixel* image::begin() const {
	return reinterpret_cast<pixel*>(data);
}
pixel* image::end() const {
	return reinterpret_cast<pixel*>(data + size);
}

void image::WriteAt(int at_x, int at_y, const image& img) {
	pixel* where = begin() + at_x + at_y * width;
	pixel* img_data = img.begin();
	for (int y = 0; y < img.height; y++) {
		memcpy(where, img_data + y * img.width, min(width - at_x, img.width) * sizeof(pixel));
		where += width;
	}
}
void image::alpha(data_t opacity) {
	for (auto& pixel : *this) {
		pixel.a = opacity;
	}
}

void image::WriteAtIfAlpha(int at_x, int at_y, const image& img) {
	pixel* where = begin() + at_x + at_y * width;
	pixel* img_data = img.begin();
	for (int y = 0; y < img.height; y++) {
		for(auto* ptr = img_data + y * img.width; ptr < ptr + min(width - at_x, img.width); ptr++)
			if(ptr->a) *where = *ptr;
		where += width;
	}
}
