//--------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef FILE_SLOW_H
#define FILE_SLOW_H

// Make the assumption of c-char strings, not UNICODE
namespace Azul
{

	class FileSlow
	{
	public:

		// Constants for the library
		static const uint32_t MAJOR_VERSION = 10;
		static const uint32_t MINOR_VERSION = 1;

		typedef void* Handle;

		enum class Mode : uint32_t
		{
			READ,
			WRITE,
			READ_WRITE
		};

		enum class Position : uint32_t
		{
			BEGIN,
			CURRENT,
			END
		};

		enum class Error : uint32_t
		{
			SUCCESS,
			OPEN_FAIL,
			CLOSE_FAIL,
			WRITE_FAIL,
			READ_FAIL,
			SEEK_FAIL,
			TELL_FAIL,
			FLUSH_FAIL,
			UNDEFINED
		};

	public:
		static FileSlow::Error Open(FileSlow::Handle &fh, const char* const fileName, FileSlow::Mode mode) noexcept;
		static FileSlow::Error Close(FileSlow::Handle &fh) noexcept;
		static FileSlow::Error Write(FileSlow::Handle fh, const void* const buffer, const DWORD inSize) noexcept;
		static FileSlow::Error Read(FileSlow::Handle fh, void* const _buffer, const DWORD _size) noexcept;
		static FileSlow::Error Seek(FileSlow::Handle fh, FileSlow::Position location, int offset) noexcept;
		static FileSlow::Error Tell(FileSlow::Handle fh, DWORD& offset) noexcept;
		static FileSlow::Error Flush(FileSlow::Handle fh) noexcept;
		static bool IsHandleValid(FileSlow::Handle fh) noexcept;

	private:
		static DWORD privGetFileDesiredAccess(FileSlow::Mode mode) noexcept;
		static DWORD privGetSeek(FileSlow::Position location) noexcept;
	};

}

#endif

// --- End of FileSlow ---
