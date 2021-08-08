#ifndef LIBIO_FILE_H

#define LIBIO_FILE_H

#include <cstdio>
#include <fcntl.h>
#include <functional>
#include <iostream>
#ifndef WIN32
#	include <sys/io.h>
#	include <sys/mman.h>
#	include <sys/stat.h>
#	include <sys/types.h>
#	include <unistd.h>
#endif
#include <cstring>
#include <vector>

namespace io {

enum class filemode {
	r,
	rb,
	w,
	wb,
	a,
	ab,
	rplus,
	rbplus,
	wplus,
	wbplus,
	aplus,
	abplus,
	rw
};

enum rd {
	words,
	lines
};

#ifdef WIN32
inline std::string __fmode_to_str(filemode mode) noexcept {
#else
inline std::string_view __fmode_to_str(filemode mode) noexcept {
#endif
	switch (mode) {
		case filemode::r: return "r";
		case filemode::rb: return "rb";
		case filemode::w: return "w";
		case filemode::wb: return "wb";
		case filemode::a: return "a";
		case filemode::ab: return "ab";
		case filemode::rplus: return "r+";
		case filemode::rbplus: return "rb+";
		case filemode::wplus: return "w+";
		case filemode::wbplus: return "wb+";
		case filemode::aplus: return "a+";
		case filemode::abplus: return "ab+";
		default: return "rw";
	}
}

template <filemode M>
concept Readable = requires { requires(M != filemode::w) && (M != filemode::wb) && (M != filemode::a) && (M != filemode::ab); };

template <filemode M>
concept Writable = requires { requires(M != filemode::r) && (M != filemode::rb); };

template <filemode M>
concept ByteMode = requires { requires(M == filemode::rb) || (M == filemode::wb) || (M == filemode::ab)
							  || (M == filemode::rbplus) || (M == filemode::wbplus) || (M == filemode::abplus)
							  || (M == filemode::rw); };

template <typename char_type_t, filemode M>
struct file {
	using string_t = std::basic_string<char_type_t>;
	using file_t   = file<char_type_t, M>;

	FILE*		handle = nullptr;
	std::string filename;
	filemode	mode   = M;
	rd			rdmode = rd::words;

	/// Opens a file and constructs a wrapper around it. on_error(this->filename) is called should the file fail to open.
	explicit file(std::string filename_, const std::function<void(const std::string&)>& on_error = {}) noexcept : filename(move(filename_)) {
		handle = fopen(filename.c_str(), __fmode_to_str(M).data());
		if (!handle) on_error(filename);
	}

	file(FILE*&& _handle, std::string _filename, filemode _mode) noexcept : filename(std::move(_filename)), mode(_mode), handle(_handle) {
		_handle = nullptr;
	}

	explicit file()			= default;
	file(const file& other) = delete;
	void operator=(const file& other) = delete;

	file(file&& other) noexcept {
		handle		 = other.handle;
		filename	 = other.filename;
		mode		 = other.mode;
		other.handle = nullptr;
		other.filename.clear();
	}

#ifndef WIN32
	/// Map the entire file into memory
	auto mmap() noexcept -> string_t requires Readable<M> {
		struct stat s {};
		int			d = fd();
		fstat(d, &s);
		char*		 mem;
		char_type_t* ptr;
		mem = (char*) ::mmap(nullptr, size_t(s.st_size), PROT_READ, MAP_PRIVATE, d, 0);
		if constexpr (std::is_same_v<char_type_t, char>) ptr = mem;
		if constexpr (std::is_same_v<char_type_t, wchar_t>) {
			ptr = (wchar_t*) calloc(s.st_size, sizeof(wchar_t));
			mbstowcs(ptr, mem, s.st_size);
		}
		string_t ret{ptr};
		munmap(mem, size_t(s.st_size));
		if constexpr (std::is_same_v<char_type_t, wchar_t>) free(ptr);
		return ret;
	}
#else
	auto ReadToEnd() noexcept -> string_t;
	auto mmap() noexcept -> string_t requires Readable<M> { return ReadToEnd(); }
#endif

	/// Read up to n bytes from the file.
	auto raw(size_t n) noexcept -> std::string requires Readable<M> && ByteMode<M> {
		std::string ret;
		ret.reserve(n);
		fread(&ret[0], 1, n, handle);
		return ret;
	}

	/// Write up to n bytes to the file.
	auto raw(void* data, size_t n_bytes) noexcept -> size_t requires Writable<M> && ByteMode<M> {
		return fwrite(data, 1, n_bytes, handle);
	}

	/// Read a single character from the file.
	auto read() noexcept -> char_type_t requires Readable<M> {
		if constexpr (std::is_same_v<char_type_t, char>)
			return char(fgetc(handle));
		if constexpr (std::is_same_v<char_type_t, wchar_t>)
			return wchar_t(fgetwc(handle));
	}

	/// Read up to n characters from the file.
	auto read(size_t n) noexcept -> string_t requires Readable<M> {
		string_t str;
		while (n-- && !eof()) str += read();
		return str;
	}

	/// Read from the file until condition is true.
	auto ReadUntil(const std::function<bool(char_type_t)>& condition) noexcept
		-> string_t requires Readable<M> {
		string_t str;
		if (!eof())
			for (;;) {
				char_type_t c = read();
				if (!eof()) str += c;
				else
					break;
				if (condition(c)) break;
			}
		return str;
	}

	/// Read a single line from the file.
	/// TODO: handle line terminators properly depending on the os
	inline auto readline() noexcept -> string_t requires Readable<M> {
		if constexpr (std::is_same_v<char_type_t, char>)
			return ReadUntil([](char c) { return c == '\n' || c == '\r'; });
		if constexpr (std::is_same_v<char_type_t, wchar_t>)
			return ReadUntil([](wchar_t c) { return c == L'\n' || c == L'\r'; });
	}

	/// Read characters up to the first character for which isspace()
	/// is true from the file.
	inline auto readword() noexcept -> string_t requires Readable<M> {
		if constexpr (std::is_same_v<char_type_t, char>)
			return ReadUntil([](char c) { return isspace(c); });
		if constexpr (std::is_same_v<char_type_t, wchar_t>)
			return ReadUntil([](wchar_t c) { return iswspace(c); });
	}

	/// Unread a single character back to the file.
	void unread(char_type_t c) noexcept requires Readable<M> {
		if constexpr (std::is_same_v<char_type_t, char>)
			ungetc(c, handle);
		if constexpr (std::is_same_v<char_type_t, wchar_t>)
			ungetwc(c, handle);
	}

	/// Read the rest of the file. Use io::file::mmap() instead on systems that support it
	auto ReadToEnd() noexcept -> string_t requires Readable<M> {
		string_t str;
		while (!eof()) str += read();
		return str;
	}

	/// Write a single character to the file.
	void write(char_type_t c) requires(M != filemode::r) && (M != filemode::rb) {
		if constexpr (std::is_same_v<char_type_t, char>)
			fputc(c, handle);
		if constexpr (std::is_same_v<char_type_t, wchar_t>)
			fputwc(c, handle);
	}

	/// Write a string to the file.
	void write(string_t str) requires Writable<M> {
		if constexpr (std::is_same_v<char_type_t, char>)
			printf("%s", str.c_str());
		if constexpr (std::is_same_v<char_type_t, wchar_t>)
			printf("%ls", str.c_str());
	}

	/// Format and write a string to the file.
	template <typename... Args>
	void writef(const string_t& format, Args... args) requires Writable<M> {
		if constexpr (std::is_same_v<char_type_t, char>)
			::fprintf(handle, format.c_str(), std::forward<Args>(args)...);
		if constexpr (std::is_same_v<char_type_t, wchar_t>)
			::fwprintf(handle, format.c_str(), std::forward<Args>(args)...);
	}

	/// Rewind this file
	inline void rewind() noexcept { ::rewind(handle); }

	/// Return this file's file descriptor
	[[nodiscard]] inline int fd() const noexcept { return fileno(handle); }

	/// Return the eof indicator for this file
	[[nodiscard]] inline bool eof() const noexcept { return ::feof(handle); }

	/// Tell the current position in the file
	[[nodiscard]] inline long tell() const noexcept { return ftell(handle); }

	/// Seek to a position in the file
	inline int seek(long off, int whence) const noexcept { return fseek(handle, off, whence); }

	[[nodiscard]] explicit inline operator bool() const noexcept { return handle; }

	/// Write c to this file
	file_t& operator<<(char_type_t c) requires Writable<M> {
		write(c);
		return *this;
	}

	/// Write str to this file
	file_t& operator<<(const string_t& str) requires Writable<M> {
		write(str);
		return *this;
	}

	/// Write the contents of other to this file
	template <filemode N>
	file_t& operator<<(file<char_type_t, N>& other) requires Writable<M> && Readable<N> {
		write(other.mmap());
		return *this;
	}

	/// Write the contents of this file to other
	template <filemode N>
	file_t& operator>>(file<char_type_t, N>& other) requires Readable<M> && Writable<N> {
		other.write(mmap());
		return *this;
	}

	/// Read a word or line from this file into buf, depending on rdmode
	file_t& operator>>(string_t& buf) requires Readable<M> {
		if (rdmode == rd::words) [[likely]]
			buf = readword();
		else [[unlikely]]
			buf = readline();
		return *this;
	}

	/// Read a character from this file into buf
	file_t& operator>>(char_type_t& c) requires Readable<M> {
		c = read();
		return *this;
	}

	/// Change the read mode for this file
	file_t& operator>>(rd _rdmode) requires Readable<M> {
		rdmode = _rdmode;
		return *this;
	}

	~file() {
		if (handle) fclose(handle);
	}

	template <typename Container>
	struct abstract_iterator {
		using iterator = typename Container::iterator;
		file_t*	  fptr;
		long	  tell;
		Container elements;

		abstract_iterator(file_t* _fptr) : fptr(_fptr), tell(_fptr->tell()) {}
		~abstract_iterator() { fptr->seek(tell, SEEK_SET); }

		virtual iterator begin() = 0;
		inline iterator	 end() { return elements.end(); }
	};

	struct char_iterator : public abstract_iterator<string_t> {
		using abstract_iterator<string_t>::abstract_iterator;
		auto begin() -> typename string_t::iterator {
			this->fptr->rewind();
			this->elements = this->fptr->mmap();
			return this->elements.begin();
		}
	};

	struct line_iterator : public abstract_iterator<std::vector<string_t>> {
		using abstract_iterator<std::vector<string_t>>::abstract_iterator;
		auto begin() -> typename std::vector<string_t>::iterator {
			this->fptr->rewind();
			while (!this->fptr->eof()) this->elements.push_back(this->fptr->readline());
			return this->elements.begin();
		}
	};

	struct word_iterator : public abstract_iterator<std::vector<string_t>> {
		using abstract_iterator<std::vector<string_t>>::abstract_iterator;
		auto begin() -> typename std::vector<string_t>::iterator {
			this->fptr->rewind();
			while (!this->fptr->eof()) this->elements.push_back(this->fptr->readword());
			return this->elements.begin();
		}
	};

	inline auto chars() -> char_iterator { return char_iterator(this); }
	inline auto lines() -> line_iterator { return line_iterator(this); }
	inline auto words() -> word_iterator { return word_iterator(this); }
};

using infile  = file<char, filemode::r>;
using winfile = file<wchar_t, filemode::r>;
using ofile	  = file<char, filemode::w>;
using wofile  = file<wchar_t, filemode::w>;
using afile	  = file<char, filemode::a>;
using wafile  = file<wchar_t, filemode::a>;

/// This function can be passed to the constructor of io::file; should any errors occur during the
/// attempt to open the file, it logs the error and calls exit(1) afterwards.
/// Calling it in any other context would make little sense.
inline void perror_and_exit(const std::string& filename) {
	std::string s = "File '" + filename + "' opened by io::file is invalid";
	perror(s.c_str());
	exit(1);
}

} // namespace io

#if 0
template <filemode FM = M,
	typename std::enable_if<
	FM == filemode::r || FM == filemode::rw || FM == filemode::rplus
	|| FM == filemode::rb || FM == filemode::rbplus,
	char_type_t>::type* = nullptr>

		template <filemode FM = M,
			typename std::enable_if<
			FM != filemode::r && FM != filemode::rb,
			char_type_t>::type* = nullptr>
#endif

#endif // CXXCOMP_FILE_H
