//
// Copyright (c) 2021 - 2024 Stephen F. Booth <me@sbooth.org>
// Part of https://github.com/sbooth/SFBAudioUtilities
// MIT license
//

#import <cctype>
#import <cstdarg>

#include <libkern/OSByteOrder.h>

#import "SFBStringFormatting.hpp"

namespace {

/// Returns true if @c i consists of four printing characters
constexpr bool fourcc_isprint(uint32_t i) noexcept
{
	return std::isprint(static_cast<unsigned char>(i)) && std::isprint(static_cast<unsigned char>(i >> 8)) && std::isprint(static_cast<unsigned char>(i >> 16)) && std::isprint(static_cast<unsigned char>(i >> 24));
}

/// Creates a @c std::string containing @c fourcc formatted as four characters and returns the result
/// @throw @c std::length_error
/// @throw @c std::bad_alloc
/// @throw @c std::bad_array_new_length
std::string fourcc_fourchar_string(uint32_t fourcc)
{
	fourcc = OSSwapHostToBigInt32(fourcc);
	std::string s(6, '\0');
	const auto n = std::snprintf(s.data(), 6 + 1, "'%.4s'", reinterpret_cast<const char *>(&fourcc));
	if(n < 0)
		return {};
	return s;
}

/// Creates a @c std::string containing @c fourcc formatted as hexadecimal and returns the result
/// @throw @c std::length_error
/// @throw @c std::bad_alloc
/// @throw @c std::bad_array_new_length
std::string fourcc_hex_string(uint32_t fourcc)
{
	char buf [10 + 1];
	const auto n = std::snprintf(buf, sizeof buf, "0x%0.8x", static_cast<int>(fourcc));
	if(n < 0)
		return {};
	return { buf, static_cast<std::string::size_type>(n) };
}

} // namespace

std::string SFB::string_from_cfstring(CFStringRef str) noexcept
{
	if(!str)
		return {};

	auto range = CFRange{ .location = 0, .length = CFStringGetLength(str) };
	auto max_size = CFStringGetMaximumSizeForEncoding(range.length, kCFStringEncodingUTF8);

	std::string result;
	result.reserve(max_size);

	char buf [128];
	while(range.length > 0) {
		CFIndex bytesWritten = 0;
		auto converted = CFStringGetBytes(str, range, kCFStringEncodingUTF8, 0, false, reinterpret_cast<UInt8 *>(buf), sizeof buf, &bytesWritten);
		result.append(buf, static_cast<std::string::size_type>(bytesWritten));

		range.location += converted;
		range.length -= converted;
	}

	return result;
}

std::string SFB::string_format(const char *format, ...)
{
	char buf [256];

	// Attempt to format to `buf`
	std::va_list args;
	va_start(args, format);
	const auto n = std::vsnprintf(buf, sizeof buf, format, args);
	va_end(args);

	// An error occurred
	if(n < 0)
		return {};

	// The formatted string fit within `buf`
	const auto len = static_cast<std::string::size_type>(n);
	if(len < sizeof buf)
		return { buf, len };

	// Allocate a new buffer and re-run the formatting
	std::string s(len, '\0');
	va_start(args, format);
	std::vsnprintf(s.data(), len + 1, format, args);
	va_end(args);

	return s;
}

std::string SFB::fourcc_string(uint32_t fourcc)
{
	if(fourcc_isprint(fourcc))
		return fourcc_fourchar_string(fourcc);
	else
		return fourcc_hex_string(fourcc);
}

std::string SFB::osstatus_string(int32_t code)
{
	if(fourcc_isprint(static_cast<uint32_t>(code))) 
		return fourcc_fourchar_string(static_cast<uint32_t>(code));
	else if(code > -200000 && code < 200000) {
		char buf [7 + 1];
		const auto n = std::snprintf(buf, sizeof buf, "%d", static_cast<int>(code));
		if(n < 0)
			return {};
		return { buf, static_cast<std::string::size_type>(n) };
	}
	else 
		return fourcc_hex_string(static_cast<uint32_t>(code));
}
