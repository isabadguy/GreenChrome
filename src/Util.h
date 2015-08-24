﻿

bool ReadMemory(PBYTE BaseAddress, PBYTE Buffer, DWORD nSize)
{
	DWORD ProtectFlag = 0;
	if (VirtualProtectEx(GetCurrentProcess(), BaseAddress, nSize, PAGE_EXECUTE_READWRITE, &ProtectFlag))
	{
		memcpy(Buffer, BaseAddress, nSize);
		VirtualProtectEx(GetCurrentProcess(), BaseAddress, nSize, ProtectFlag, &ProtectFlag);
		return true;
	}
	return false;
}

bool WriteMemory(PBYTE BaseAddress, PBYTE Buffer, DWORD nSize)
{
	DWORD ProtectFlag = 0;
	if (VirtualProtectEx(GetCurrentProcess(), BaseAddress, nSize, PAGE_EXECUTE_READWRITE, &ProtectFlag))
	{
		memcpy(BaseAddress, Buffer, nSize);
		FlushInstructionCache(GetCurrentProcess(), BaseAddress, nSize);
		VirtualProtectEx(GetCurrentProcess(), BaseAddress, nSize, ProtectFlag, &ProtectFlag);
		return true;
	}
	return false;
}

#define MAX_SIZE 32767

//如果需要给路径加引号
inline std::wstring QuotePathIfNeeded(const std::wstring &path)
{
	std::vector<wchar_t> buffer(path.length() + 1 /* null */ + 2 /* quotes */);
	wcscpy(&buffer[0], path.c_str());

	PathQuoteSpaces(&buffer[0]);

	return std::wstring(&buffer[0]);
}

//展开环境路径比如 %windir%
std::wstring ExpandEnvironmentPath(const std::wstring &path)
{
	std::vector<wchar_t> buffer(MAX_PATH);
	size_t expandedLength = ::ExpandEnvironmentStrings(path.c_str(), &buffer[0], buffer.size());
	if (expandedLength > buffer.size())
	{
		buffer.resize(expandedLength);
		expandedLength = ::ExpandEnvironmentStrings(path.c_str(), &buffer[0], buffer.size());
	}
	return std::wstring(&buffer[0], 0, expandedLength);
}

//替换字符串
void ReplaceStringInPlace(std::wstring& subject, const std::wstring& search, const std::wstring& replace)
{
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::wstring::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
}

