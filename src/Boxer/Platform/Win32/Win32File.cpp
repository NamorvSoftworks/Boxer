#ifdef PLATFORM_WINDOWS

#include <Boxer/File.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

namespace boxer {
	FileBlock::FileBlock(String path) {
		_Path = path;

		wchar_t* wide_path = path.Unicode();
		Defer(delete[] wide_path);

		_Handle = CreateFile(wide_path, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
		if (_Handle == INVALID_HANDLE_VALUE) {
			// TODO: We need logging!!
			__debugbreak();
		}

		LARGE_INTEGER FileSize;
		if (!GetFileSizeEx(_Handle, &FileSize)) {
			// TODO: We need logging!!
			__debugbreak();
		}

		HANDLE FileMapping = CreateFileMappingA(_Handle, 0, PAGE_WRITECOPY, 0, 0, 0);
		if (FileMapping == nullptr) {
			// TODO: We need logging!!
			__debugbreak();
		}

		char* FileMemory = (char*)MapViewOfFileEx(FileMapping, FILE_MAP_COPY, 0, 0, 0, 0);
		if (FileMemory == nullptr) {
			// TODO: We need logging!!
			__debugbreak();
		}

		_Data = String(FileMemory, (size_t)FileSize.QuadPart);
	}

	FileBlock::~FileBlock() {
		UnmapViewOfFile(_Data.Data());
		CloseHandle(_Handle);
	}
}

#endif /* PLATFORM_WINDOWS */
