/*
 * Copyright (c) 2021 Stephen F. Booth <me@sbooth.org>
 * MIT license
 */

#pragma once

#import <cctype>
#import <cstring>
#import <exception>

class SFBCAException : public std::exception
{

public:

	static inline void ThrowIfError(OSStatus result, const char * const operation = nullptr) noexcept(false)
	{
		if(__builtin_expect(result != noErr, 0))
			throw SFBCAException(result, operation);
	}

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

	inline SFBCAException(const SFBCAException& rhs) noexcept
	{
		*this = rhs;
	}

	inline SFBCAException& operator=(const SFBCAException& rhs) noexcept
	{
		mError = rhs.mError;
		strlcpy(mOperation, rhs.mOperation, sizeof(mOperation));
		strlcpy(mWhat, rhs.mWhat, sizeof(mWhat));
		return *this;
	}

	inline OSStatus Error() const noexcept
	{
		return mError;
	}

	inline const char * const Operation() const noexcept
	{
		return mOperation;
	}

	virtual const char * what() const noexcept
	{
		return mWhat;
	}

private:

	OSStatus mError;
	char mOperation [128];
	char mWhat [16];

};
