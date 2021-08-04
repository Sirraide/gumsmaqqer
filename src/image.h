#ifndef GUMSMAQQER_IMAGE_H
#define GUMSMAQQER_IMAGE_H

#include <string>
#include <vector>

using data_t = unsigned char;

struct __attribute__((packed)) pixel {
	data_t r;
	data_t g;
	data_t b;
	data_t a;
};
static_assert(sizeof(pixel) == 4 * sizeof(data_t));

struct image {
	int		width{};
	int		height{};
	int		channels{};
	int		size = 0;
	data_t* data{};
	bool	alloc = false;

	explicit image(const std::string& filename);
	explicit image(int width = 0, int height = 0, int channels = 4);
	image(const image& other);
	image(image&& other) noexcept;
	image&  operator=(const image& other);
	image& operator=(image&& other) noexcept;

	void save(const std::string& filename) const;
	void WriteAt(int at_x, int at_y, const image& rows);
	void WriteAtIfAlpha(int at_x, int at_y, const image& rows) const asm("WriteIfAlpha");
	void alpha(data_t opacity = 255);

	void checksize();

	[[nodiscard]] pixel* begin() const;
	[[nodiscard]] pixel* end() const;

	~image();
};

#endif // GUMSMAQQER_IMAGE_H
