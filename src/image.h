#ifndef GUMSMAQQER_IMAGE_H
#define GUMSMAQQER_IMAGE_H

#include <string>
#include <vector>

using data_t = unsigned char;

#ifdef WIN32
#	define PACKED
__pragma(pack(push, 1))
#else
#	define PACKED __attribute__((packed))
#endif
	struct PACKED pixel {
	data_t r;
	data_t g;
	data_t b;
	data_t a;
};
#ifdef WIN32
__pragma(pack(pop))
#endif

	static_assert(sizeof(pixel) == 4 * sizeof(data_t));

struct image {
	int						   width{};
	int						   height{};
	static constexpr const int channels = 4;
	int						   size		= 0;
	data_t*					   data{};
	bool					   alloc = false;

	explicit image(const std::string& filename);
	explicit image(int width = 0, int height = 0);
	image(const image& other);
	image(image&& other) noexcept;
	image& operator=(const image& other);
	image& operator=(image&& other) noexcept;

	void save(const std::string& filename) const;
	void WriteAt(int at_x, int at_y, const image& rows);
	void WriteAtIfAlpha(int at_x, int at_y, const image& rows) const;
	void alpha(data_t opacity = 255);

	void checksize() const;

	[[nodiscard]] pixel* begin() const;
	[[nodiscard]] pixel* end() const;

	~image();
};

#endif // GUMSMAQQER_IMAGE_H
