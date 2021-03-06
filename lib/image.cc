#include "image.h"

#include "../lib/io.h"
// clang-format off
/// required because stb_image_write uses a variable called 'Y'
#undef Y
#define STB_IMAGE_IMPLEMENTATION
#include "../lib/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../lib/stb_image_write.h"
// clang-format on
#include "utils.h"

#include <cstdlib>

using namespace std;

namespace gsmq {

image::image(const std::string& filename) {
	io::file<char, io::filemode::rb>(filename, io::perror_and_exit);
	int ch;

	if (data = stbi_load(filename.c_str(), &width, &height, &ch, 0), !data)
		fatal(RED, "Internal error: Could not load image from file " G + filename + R);
	if (ch != channels)
		fatal(RED, "BAD FILE (" G + to_string(ch) + RED "): MUST NEEDS SPECIFY FILE IN PNG FORMAT" R);
	size = width * height * channels;
	__check_size();
}
image::image(int _width, int _height) : width(_width), height(_height) {
	size  = width * height * channels;
	alloc = true;
	data  = static_cast<data_t*>(calloc(size, 1));
	__check_size();
}

image::image(const image& other) : image(other.width, other.height) {
	memcpy(data, other.data, other.size);
}

image::image(image&& other) noexcept {
	*this = move(other);
	__check_size();
}

image& image::operator=(const image& other) {
	image copy{other};
	*this = move(copy);
	return *this;
}
image& image::operator=(image&& other) noexcept {
	width	   = other.width;
	height	   = other.height;
	data	   = other.data;
	size	   = other.size;
	nofree	   = other.nofree;
	alloc	   = other.alloc;
	other.data = nullptr;
	return *this;
}

image::~image() {
	if (data && !nofree) {
		if (alloc) free(data);
		else
			stbi_image_free(data);
	}
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
	pixel* where	= begin() + at_x + at_y * width;
	pixel* img_data = img.begin();
	int	   m_height = min(height - at_y, img.height);
	int	   m_width	= min(width - at_x, img.width);
	for (int y = 0; y < m_height; y++) {
		memcpy(where, img_data + y * img.width, m_width * sizeof(pixel));
		where += width;
	}
}
void image::alpha(data_t opacity) {
	for (auto& pixel : *this) {
		pixel.a = opacity;
	}
}

void CXX_WriteIfAlpha(pixel* img_data, pixel* where_start,
	int src_width, int dest_width, int m_height) {
	for (int y = 0; y < m_height; y++) {
		pixel* x_offs = img_data + y * src_width;
		pixel *ptr = x_offs, *where = where_start + y * dest_width;
		for (; ptr < x_offs + (src_width / 8) * 8; ptr += 8, where += 8) {
			if (ptr[0].a) [[likely]]
				where[0] = ptr[0];
			if (ptr[1].a) [[likely]]
				where[1] = ptr[1];
			if (ptr[2].a) [[likely]]
				where[2] = ptr[2];
			if (ptr[3].a) [[likely]]
				where[3] = ptr[3];
			if (ptr[4].a) [[likely]]
				where[4] = ptr[4];
			if (ptr[5].a) [[likely]]
				where[5] = ptr[5];
			if (ptr[6].a) [[likely]]
				where[6] = ptr[6];
			if (ptr[7].a) [[likely]]
				where[7] = ptr[7];
		}
		for (; ptr < x_offs + src_width; ptr++, where++)
			if (ptr->a) *where = *ptr;
	}
}

void image::WriteAtIfAlpha(int at_x, int at_y, const image& img) const {
	pixel* where_start = begin() + at_x + at_y * width;
	pixel* img_data	   = img.begin();
	int	   m_height	   = min(height - at_y, img.height);
	CXX_WriteIfAlpha(img_data, where_start, img.width, width, m_height);
}
void image::__check_size() const {
	if (size < 0) fatal(RED, "Internal error: Image size may not be less than zero!" R);
}

} // namespace gsmq