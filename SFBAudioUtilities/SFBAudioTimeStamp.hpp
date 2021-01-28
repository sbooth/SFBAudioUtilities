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

	/*! @brief Creates a new, empty \c SFBAudioTimeStamp */
	inline SFBAudioTimeStamp() noexcept
	{
		memset(this, 0, sizeof(AudioTimeStamp));
	}

	/*! @brief Creates a new \c SFBAudioTimeStamp for the specified \c AudioTimeStamp */
	inline SFBAudioTimeStamp(const AudioTimeStamp& timeStamp) noexcept
	{
		memcpy(this, &timeStamp, sizeof(AudioTimeStamp));
	}

	/*! @brief Creates a new \c SFBAudioTimeStamp with the specified sample time */
	inline SFBAudioTimeStamp(Float64 sampleTime) noexcept
	: SFBAudioTimeStamp()
	{
		mSampleTime = sampleTime;
		mFlags = kAudioTimeStampSampleTimeValid;
	}

	/*! @brief Creates a new \c SFBAudioTimeStamp with the specified host time */
	inline SFBAudioTimeStamp(UInt64 hostTime) noexcept
	: SFBAudioTimeStamp()
	{
		mHostTime = hostTime;
		mFlags = kAudioTimeStampHostTimeValid;
	}

	/*! @brief Creates a new \c SFBAudioTimeStamp with the specified sample and host times */
	inline SFBAudioTimeStamp(Float64 sampleTime, UInt64 hostTime) noexcept
	: SFBAudioTimeStamp()
	{
		mSampleTime = sampleTime;
		mHostTime = hostTime;
		mFlags = kAudioTimeStampSampleTimeValid | kAudioTimeStampHostTimeValid;

	}

	/*! @brief Creates a new \c SFBAudioTimeStamp with the specified sample and host times and rate scalar */
	inline SFBAudioTimeStamp(Float64 sampleTime, UInt64 hostTime, Float64 rateScalar) noexcept
	: SFBAudioTimeStamp()
	{
		mSampleTime = sampleTime;
		mHostTime = hostTime;
		mRateScalar = rateScalar;
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
	/*! @name Comparison */
	//@{

	/*! @brief Compare two \c SFBAudioTimeStamp objects for equality*/
	bool operator==(const SFBAudioTimeStamp& rhs) const noexcept;

	/*! @brief Compare two \c SFBAudioTimeStamp objects for inequality*/
	inline bool operator!=(const SFBAudioTimeStamp& rhs) const noexcept
	{
		return !operator==(rhs);
	}

	/*! @brief Compare two \c SFBAudioTimeStamp objects */
	bool operator<(const SFBAudioTimeStamp& rhs) const noexcept;

	/*! @brief Compare two \c SFBAudioTimeStamp objects */
	inline bool operator<=(const SFBAudioTimeStamp& rhs) const noexcept
	{
		return operator<(rhs) || operator==(rhs);
	}

	/*! @brief Compare two \c SFBAudioTimeStamp objects */
	inline bool operator>=(const SFBAudioTimeStamp& rhs) const noexcept
	{
		return !operator<(rhs);
	}

	/*! @brief Compare two \c SFBAudioTimeStamp objects */
	inline bool operator>(const SFBAudioTimeStamp& rhs) const noexcept
	{
		return !(operator<(rhs) || operator==(rhs));
	}

	//@}

	// ========================================
	/*! @name Flags */
	//@{

	/*! Returns \c true if the \c SFBAudioTimeStamp is valid */
	inline bool IsValid() const noexcept
	{
		return mFlags != kAudioTimeStampNothingValid;
	}

	/*! Returns \c true if \c mSampleTime is valid */
	inline bool SampleTimeIsValid() const noexcept
	{
		return (mFlags & kAudioTimeStampSampleTimeValid) == kAudioTimeStampSampleTimeValid;
	}

	/*! Returns \c true if \c mHostTime is valid */
	inline bool HostTimeIsValid() const noexcept
	{
		return (mFlags & kAudioTimeStampHostTimeValid) == kAudioTimeStampHostTimeValid;
	}

	/*! Returns \c true if \c mRateScalar is valid */
	inline bool RateScalarIsValid() const noexcept
	{
		return (mFlags & kAudioTimeStampRateScalarValid) == kAudioTimeStampRateScalarValid;
	}

	/*! Returns \c true if \c mWordClockTime is valid */
	inline bool WordClockTimeIsValid() const noexcept
	{
		return (mFlags & kAudioTimeStampWordClockTimeValid) == kAudioTimeStampWordClockTimeValid;
	}

	/*! Returns \c true if \c mSMPTETime is valid */
	inline bool SMPTETimeIsValid() const noexcept
	{
		return (mFlags & kAudioTimeStampSMPTETimeValid) == kAudioTimeStampSMPTETimeValid;
	}

	//@}

};
