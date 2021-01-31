//
// Copyright (c) 2021 Stephen F. Booth <me@sbooth.org>
// MIT license
//

#include "SFBAudioTimeStamp.hpp"

bool SFBAudioTimeStamp::operator==(const AudioTimeStamp& rhs) const noexcept
{
	if(SampleTimeIsValid() && (rhs.mFlags & kAudioTimeStampSampleTimeValid))
		return mSampleTime == rhs.mSampleTime;

	if(HostTimeIsValid() && (rhs.mFlags & kAudioTimeStampHostTimeValid))
		return mHostTime == rhs.mHostTime;

	if(WordClockTimeIsValid() && (rhs.mFlags & kAudioTimeStampWordClockTimeValid))
		return mWordClockTime == rhs.mWordClockTime;

	return false;
}

bool SFBAudioTimeStamp::operator<(const AudioTimeStamp& rhs) const noexcept
{
	if(SampleTimeIsValid() && (rhs.mFlags & kAudioTimeStampSampleTimeValid))
		return mSampleTime < rhs.mSampleTime;

	if(HostTimeIsValid() && (rhs.mFlags & kAudioTimeStampHostTimeValid))
		return mHostTime < rhs.mHostTime;

	if(WordClockTimeIsValid() && (rhs.mFlags & kAudioTimeStampWordClockTimeValid))
		return mWordClockTime < rhs.mWordClockTime;

	return false;
}
