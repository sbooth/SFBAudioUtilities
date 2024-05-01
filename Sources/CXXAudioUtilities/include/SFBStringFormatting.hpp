//
// Copyright (c) 2021 - 2024 Stephen F. Booth <me@sbooth.org>
// Part of https://github.com/sbooth/SFBAudioUtilities
// MIT license
//

#pragma once

#import <string>

#import <CoreFoundation/CoreFoundation.h>

namespace SFB {

/// Creates a @c std::string from a @c CFString using UTF-8 and returns the result
/// @throw @c std::length_error
/// @throw @c std::bad_alloc
/// @throw @c std::bad_array_new_length
/// @param str A @c CFString containing the characters to copy
/// @return A @c std::string containing the contents of @c str in UTF-8
std::string string_from_cfstring(CFStringRef str) noexcept;

/// Creates a @c std::string using a @c std::printf format string and returns the result
/// @throw @c std::length_error
/// @throw @c std::bad_alloc
/// @throw @c std::bad_array_new_length
/// @param format A null-terminated @c std::printf format string specifying how to interpret the variable arguments
/// @return A @c std::string containing the formatted result or an empty string if a parse error occurred
std::string string_format(const char *format, ...) __attribute__((format (printf, 1, 2)));

/// Creates a string representation of a four-character code and returns the result
/// @throw @c std::length_error
/// @throw @c std::bad_alloc
/// @throw @c std::bad_array_new_length
/// @param fourcc A four-character code
/// @return A @c std::string containing a string representation of @c fourcc
std::string fourcc_string(uint32_t fourcc);

/// Creates a string representation of an @c OSStatus result code and returns the result
/// @throw @c std::length_error
/// @throw @c std::bad_alloc
/// @throw @c std::bad_array_new_length
/// @param code An @c OSStatus result code
/// @return A @c std::string containing a string representation of @c code
std::string osstatus_string(int32_t code);

} // namespace SFB
