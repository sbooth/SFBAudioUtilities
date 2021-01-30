//
// Copyright (c) 2021 Stephen F. Booth <me@sbooth.org>
// MIT license
//

#include "SFBAudioTimeStamp.hpp"

bool SFBAudioTimeStamp::operator==(const SFBAudioTimeStamp& rhs) const noexcept
{
	if(SampleTimeIsValid() && rhs.SampleTimeIsValid())
		return mSampleTime == rhs.mSampleTime;

	if(HostTimeIsValid() && rhs.HostTimeIsValid())
		return mHostTime == rhs.mHostTime;

	if(WordClockTimeIsValid() && rhs.WordClockTimeIsValid())
		return mWordClockTime == rhs.mWordClockTime;

	return false;
}

bool SFBAudioTimeStamp::operator<(const SFBAudioTimeStamp& rhs) const noexcept
{
	if(SampleTimeIsValid() && rhs.SampleTimeIsValid())
		return mSampleTime < rhs.mSampleTime;

	if(HostTimeIsValid() && rhs.HostTimeIsValid())
		return mHostTime < rhs.mHostTime;

	if(WordClockTimeIsValid() && rhs.WordClockTimeIsValid())
		return mWordClockTime < rhs.mWordClockTime;

	return false;
}
