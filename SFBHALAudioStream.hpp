//
// Copyright (c) 2021 Stephen F. Booth <me@sbooth.org>
// MIT license
//

#pragma once

#import "SFBHALAudioObject.hpp"

class SFBHALAudioStream : public SFBHALAudioObject
{

public:

	/// Creates an unknown @c SFBHALAudioStream
	SFBHALAudioStream() noexcept = default;

	/// Copy constructor
	SFBHALAudioStream(const SFBHALAudioStream& rhs) noexcept = default;

	/// Assignment operator
	SFBHALAudioStream& operator=(const SFBHALAudioStream& rhs) noexcept = default;

	/// Destructor
	virtual ~SFBHALAudioStream() = default;

	/// Move constructor
	SFBHALAudioStream(SFBHALAudioStream&& rhs) = default;

	/// Move assignment operator
	SFBHALAudioStream& operator=(SFBHALAudioStream&& rhs) = default;


	/// Creates a @c SFBHALAudioStream with the specified objectID
	inline SFBHALAudioStream(AudioObjectID objectID) noexcept
	: SFBHALAudioObject(objectID)
	{}


	inline UInt32 Latency() const
	{
		return IntegralProperty<UInt32>(SFBAudioObjectPropertyAddress(kAudioStreamPropertyLatency));
	}

};
