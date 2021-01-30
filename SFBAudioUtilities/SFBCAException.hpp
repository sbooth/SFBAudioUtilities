//
// Copyright (c) 2021 Stephen F. Booth <me@sbooth.org>
// MIT license
//

#pragma once

#import <cctype>
#import <cstring>
#import <exception>

class SFBCAException : public std::exception
{

public:

	/// Throws an @c SFBCAException if @c result!=noErr
	/// @param result An @c OSStatus result code
	/// @param operation An optional string describing the operation producing @c result
	/// @throw @c SFBCAException
	static inline void ThrowIfError(OSStatus result, const char * const operation = nullptr)
	{
		if(__builtin_expect(result != noErr, 0))
			throw SFBCAException(result, operation);
	}

	/// Creates a new @c SFBCAException
	/// @param error An @c OSStatus error code
	/// @param operation An optional string describing the operation producing @c error
	inline SFBCAException(OSStatus error, const char * const operation = nullptr) noexcept
	: mError(error)
	{
		if(operation)
			strlcpy(mOperation, operation, sizeof(mOperation));
		// Missing C11 function:
//			strcpy_s(mOperation, sizeof(mOperation), operation);
		else
			mOperation[0] = '\0';

		auto err = CFSwapInt32HostToBig(mError);
		std::memcpy(&mWhat[0] + 1, &err, 4);
		if(std::isprint(mWhat[1]) && std::isprint(mWhat[2]) && std::isprint(mWhat[3]) && std::isprint(mWhat[4])) {
			mWhat[0] = mWhat[5] = '\'';
			mWhat[6] = '\0';
		}
		else if(mError > -200000 && mError < 200000)
			snprintf(mWhat, sizeof(mWhat), "%d", static_cast<int>(mError));
		else
			snprintf(mWhat, sizeof(mWhat), "0x%x", static_cast<int>(mError));
	}

	/// Copy constructor
	inline SFBCAException(const SFBCAException& rhs) noexcept
	{
		*this = rhs;
	}

	/// Assignment operator
	inline SFBCAException& operator=(const SFBCAException& rhs) noexcept
	{
		if(this != &rhs) {
			mError = rhs.mError;
			strlcpy(mOperation, rhs.mOperation, sizeof(mOperation));
			strlcpy(mWhat, rhs.mWhat, sizeof(mWhat));
		}
		return *this;
	}

	/// Returns the associated @c OSStatus error code
	inline OSStatus Error() const noexcept
	{
		return mError;
	}

	/// Returns the operation producing the error
	inline const char * const Operation() const noexcept
	{
		return mOperation;
	}

	/// Returns the four-char code or string representation of the numeric @c OSStatus error code
	virtual const char * what() const noexcept
	{
		return mWhat;
	}

private:

	/// The associated @c OSStatus error code
	OSStatus mError;
	/// The operation producing @c mError
	char mOperation [128];
	/// The four-char code or string representation of @c mError
	char mWhat [16];

};
