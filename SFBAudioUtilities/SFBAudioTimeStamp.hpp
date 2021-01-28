/*
 * Copyright (c) 2021 Stephen F. Booth <me@sbooth.org>
 * MIT license
 */

#pragma once

#import <CoreAudio/CoreAudioTypes.h>

/*! @brief A class extending the functionality of a Core %Audio \c AudioTimeStamp */
class SFBAudioTimeStamp : public AudioTimeStamp
{

public:

	// ========================================
	/*! @name Creation and Destruction */
	//@{

	/*! @brief Create a new, empty \c SFBAudioTimeStamp */
	inline SFBAudioTimeStamp() noexcept
	{
		memset(this, 0, sizeof(AudioTimeStamp));
	}

	/*! @brief Create a new \c SFBAudioTimeStamp for the specified \c AudioTimeStamp */
	inline SFBAudioTimeStamp(const AudioTimeStamp& timeStamp) noexcept
	{
		memcpy(this, &timeStamp, sizeof(AudioTimeStamp));
	}

	SFBAudioTimeStamp(Float64 sampleTime) noexcept
	: SFBAudioTimeStamp()
	{
		mSampleTime = sampleTime;
		mFlags = kAudioTimeStampSampleTimeValid;
	}

	SFBAudioTimeStamp(UInt64 hostTime) noexcept
	: SFBAudioTimeStamp()
	{
		mHostTime = hostTime;
		mFlags = kAudioTimeStampHostTimeValid;
	}

	SFBAudioTimeStamp(Float64 sampleTime, UInt64 hostTime) noexcept
	: SFBAudioTimeStamp()
	{
		mSampleTime = sampleTime;
		mHostTime = hostTime;
		mFlags = kAudioTimeStampSampleTimeValid | kAudioTimeStampHostTimeValid;

	}

	SFBAudioTimeStamp(Float64 sampleTime, UInt64 hostTime, Float64 inRateScalar) noexcept
	: SFBAudioTimeStamp()
	{
		mSampleTime = sampleTime;
		mHostTime = hostTime;
		mRateScalar = inRateScalar;
		mFlags = kAudioTimeStampSampleTimeValid | kAudioTimeStampHostTimeValid | kAudioTimeStampRateScalarValid;
	}

	/*! @brief Copy constructor */
	inline SFBAudioTimeStamp(const SFBAudioTimeStamp& rhs) noexcept
	{
		*this = rhs;
	}

	/*! @brief Assignment operator */
	inline SFBAudioTimeStamp& operator=(const AudioTimeStamp& rhs) noexcept
	{
		memcpy(this, &rhs, sizeof(AudioTimeStamp));
		return *this;
	}

	//@}

	// ========================================
	/*! @name Flags */
	//@{

	inline bool SampleTimeIsValid() const noexcept
	{
		return (mFlags & kAudioTimeStampSampleTimeValid) == kAudioTimeStampSampleTimeValid;
	}

	inline bool HostTimeIsValid() const noexcept
	{
		return (mFlags & kAudioTimeStampHostTimeValid) == kAudioTimeStampHostTimeValid;
	}

	//@}

};

constexpr bool operator<(const AudioTimeStamp& lhs, const AudioTimeStamp& rhs) noexcept;
constexpr bool operator==(const AudioTimeStamp& lhs, const AudioTimeStamp& rhs) noexcept;

inline constexpr bool operator!=(const AudioTimeStamp& lhs, const AudioTimeStamp& rhs) noexcept
{
	return !(lhs == rhs);
}

inline constexpr bool operator<=(const AudioTimeStamp& lhs, const AudioTimeStamp& rhs) noexcept
{
	return (lhs < rhs) || (lhs == rhs);
}

inline constexpr bool operator>=(const AudioTimeStamp& lhs, const AudioTimeStamp& rhs) noexcept
{
	return !(lhs < rhs);
}

inline constexpr bool operator>(const AudioTimeStamp& lhs, const AudioTimeStamp& rhs) noexcept
{
	return !((lhs < rhs) || (lhs == rhs));
}
