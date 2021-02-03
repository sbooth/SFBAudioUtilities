//
// Copyright (c) 2021 Stephen F. Booth <me@sbooth.org>
// MIT license
//

#pragma once

#import <cstring>

#import <CoreAudioTypes/CoreAudioTypes.h>

/// A class extending the functionality of a Core Audio @c AudioTimeStamp
class SFBAudioTimeStamp : public AudioTimeStamp
{

public:

#pragma mark Creation and Destruction

	/// Creates an empty @c SFBAudioTimeStamp
	SFBAudioTimeStamp() noexcept = default;

	/// Copy constructor
	SFBAudioTimeStamp(const SFBAudioTimeStamp& rhs) noexcept = default;

	/// Assignment operator
	SFBAudioTimeStamp& operator=(const SFBAudioTimeStamp& rhs) noexcept = default;

	/// Destructor
	~SFBAudioTimeStamp() = default;

	/// Move constructor
	SFBAudioTimeStamp(SFBAudioTimeStamp&& rhs) noexcept = default;

	/// Move assignment operator
	SFBAudioTimeStamp& operator=(SFBAudioTimeStamp&& rhs) noexcept = default;


	/// Creates a new @c SFBAudioTimeStamp with the specified sample time
	inline SFBAudioTimeStamp(Float64 sampleTime) noexcept
	: AudioTimeStamp{}
	{
		mSampleTime = sampleTime;
		mFlags = kAudioTimeStampSampleTimeValid;
	}

	/// Creates a new @c SFBAudioTimeStamp with the specified host time
	inline SFBAudioTimeStamp(UInt64 hostTime) noexcept
	: AudioTimeStamp{}
	{
		mHostTime = hostTime;
		mFlags = kAudioTimeStampHostTimeValid;
	}

	/// Creates a new @c SFBAudioTimeStamp with the specified sample and host times
	inline SFBAudioTimeStamp(Float64 sampleTime, UInt64 hostTime) noexcept
	: AudioTimeStamp{}
	{
		mSampleTime = sampleTime;
		mHostTime = hostTime;
		mFlags = kAudioTimeStampSampleTimeValid | kAudioTimeStampHostTimeValid;

	}

	/// Creates a new @c SFBAudioTimeStamp with the specified sample and host times and rate scalar
	inline SFBAudioTimeStamp(Float64 sampleTime, UInt64 hostTime, Float64 rateScalar) noexcept
	: AudioTimeStamp{}
	{
		mSampleTime = sampleTime;
		mHostTime = hostTime;
		mRateScalar = rateScalar;
		mFlags = kAudioTimeStampSampleTimeValid | kAudioTimeStampHostTimeValid | kAudioTimeStampRateScalarValid;
	}

	// Native overloads

	/// Creates a new @c SFBAudioTimeStamp for the specified @c AudioTimeStamp
	inline SFBAudioTimeStamp(const AudioTimeStamp& rhs) noexcept
	: AudioTimeStamp{rhs}
	{}

	/// Assignment operator
	inline SFBAudioTimeStamp& operator=(const AudioTimeStamp& rhs) noexcept
	{
		AudioTimeStamp::operator=(rhs);
		return *this;
	}

#pragma mark Comparison

	/// Returns @c true if @c rhs is equal to @c this
	bool operator==(const AudioTimeStamp& rhs) const noexcept;

	/// Returns @c true if @c rhs is not equal to @c this
	inline bool operator!=(const AudioTimeStamp& rhs) const noexcept
	{
		return !operator==(rhs);
	}

	/// Returns @c true if @c rhs is less than @c this
	bool operator<(const AudioTimeStamp& rhs) const noexcept;

	/// Returns @c true if @c rhs is less than or equal to @c this
	inline bool operator<=(const AudioTimeStamp& rhs) const noexcept
	{
		return operator<(rhs) || operator==(rhs);
	}

	/// Returns @c true if @c rhs is greater than or equal to @c this
	inline bool operator>=(const AudioTimeStamp& rhs) const noexcept
	{
		return !operator<(rhs);
	}

	/// Returns @c true if @c rhs is greater than @c this
	inline bool operator>(const AudioTimeStamp& rhs) const noexcept
	{
		return !(operator<(rhs) || operator==(rhs));
	}

#pragma mark Flags

	/// Returns @c true if the @c SFBAudioTimeStamp is valid
	inline explicit operator bool() const noexcept
	{
		return mFlags != kAudioTimeStampNothingValid;
	}

	/// Returns @c true if the @c SFBAudioTimeStamp is not valid
	inline bool operator!() const noexcept
	{
		return !operator bool();
	}

	/// Returns @c true if the @c SFBAudioTimeStamp is valid
	inline bool IsValid() const noexcept
	{
		return operator bool();
	}

	/// Returns @c true if @c mSampleTime is valid
	inline bool SampleTimeIsValid() const noexcept
	{
		return (mFlags & kAudioTimeStampSampleTimeValid) == kAudioTimeStampSampleTimeValid;
	}

	/// Returns @c true if @c mHostTime is valid
	inline bool HostTimeIsValid() const noexcept
	{
		return (mFlags & kAudioTimeStampHostTimeValid) == kAudioTimeStampHostTimeValid;
	}

	/// Returns @c true if @c mRateScalar is valid
	inline bool RateScalarIsValid() const noexcept
	{
		return (mFlags & kAudioTimeStampRateScalarValid) == kAudioTimeStampRateScalarValid;
	}

	/// Returns @c true if @c mWordClockTime is valid
	inline bool WordClockTimeIsValid() const noexcept
	{
		return (mFlags & kAudioTimeStampWordClockTimeValid) == kAudioTimeStampWordClockTimeValid;
	}

	/// Returns @c true if @c mSMPTETime is valid
	inline bool SMPTETimeIsValid() const noexcept
	{
		return (mFlags & kAudioTimeStampSMPTETimeValid) == kAudioTimeStampSMPTETimeValid;
	}

};
