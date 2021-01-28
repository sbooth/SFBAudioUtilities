/*
 * Copyright (c) 2021 Stephen F. Booth <me@sbooth.org>
 * MIT license
 */

#include "SFBAudioTimeStamp.hpp"

bool operator<(const AudioTimeStamp& lhs, const AudioTimeStamp& rhs) noexcept
{
	if((lhs.mFlags & kAudioTimeStampSampleTimeValid) && (rhs.mFlags & kAudioTimeStampSampleTimeValid))
		return lhs.mSampleTime < rhs.mSampleTime;

	if((lhs.mFlags & kAudioTimeStampHostTimeValid) && (rhs.mFlags & kAudioTimeStampHostTimeValid))
		return lhs.mHostTime < rhs.mHostTime;

	if((lhs.mFlags & kAudioTimeStampWordClockTimeValid) && (rhs.mFlags & kAudioTimeStampWordClockTimeValid))
		return lhs.mWordClockTime < rhs.mWordClockTime;

	return false;
}

bool operator==(const AudioTimeStamp& lhs, const AudioTimeStamp& rhs) noexcept
{
	if((lhs.mFlags & kAudioTimeStampSampleTimeValid) && (rhs.mFlags & kAudioTimeStampSampleTimeValid))
		return lhs.mSampleTime == rhs.mSampleTime;

	if((lhs.mFlags & kAudioTimeStampHostTimeValid) && (rhs.mFlags & kAudioTimeStampHostTimeValid))
		return lhs.mHostTime == rhs.mHostTime;

	if((lhs.mFlags & kAudioTimeStampWordClockTimeValid) && (rhs.mFlags & kAudioTimeStampWordClockTimeValid))
		return lhs.mWordClockTime == rhs.mWordClockTime;

	return false;
}
