#pragma once

class iNonCopyable
{
public:
	iNonCopyable() = default;

	iNonCopyable(const iNonCopyable&) = delete;
	iNonCopyable& operator=(const iNonCopyable&) = delete;

};
