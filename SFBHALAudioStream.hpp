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
	constexpr SFBHALAudioStream() noexcept = default;

	/// Copy constructor
	constexpr SFBHALAudioStream(const SFBHALAudioStream& rhs) noexcept = default;

	/// Assignment operator
	SFBHALAudioStream& operator=(const SFBHALAudioStream& rhs) noexcept = default;

	/// Destructor
	virtual ~SFBHALAudioStream() = default;

	/// Move constructor
	SFBHALAudioStream(SFBHALAudioStream&& rhs) = default;

	/// Move assignment operator
	SFBHALAudioStream& operator=(SFBHALAudioStream&& rhs) = default;


	/// Creates a @c SFBHALAudioStream with the specified objectID
	inline constexpr SFBHALAudioStream(AudioObjectID objectID) noexcept
	: SFBHALAudioObject(objectID)
	{}

	inline bool IsActive() const
	{
		return ArithmeticProperty<UInt32>(SFBAudioObjectPropertyAddress(kAudioStreamPropertyIsActive)) != 0;
	}

	inline UInt32 Direction() const
	{
		return ArithmeticProperty<UInt32>(SFBAudioObjectPropertyAddress(kAudioStreamPropertyDirection));
	}

	inline UInt32 TerminalType() const
	{
		return ArithmeticProperty<UInt32>(SFBAudioObjectPropertyAddress(kAudioStreamPropertyTerminalType));
	}

	inline UInt32 StartingChannel() const
	{
		return ArithmeticProperty<UInt32>(SFBAudioObjectPropertyAddress(kAudioStreamPropertyStartingChannel));
	}

	inline UInt32 Latency() const
	{
		return ArithmeticProperty<UInt32>(SFBAudioObjectPropertyAddress(kAudioStreamPropertyLatency));
	}

	inline SFBAudioStreamBasicDescription VirtualFormat() const
	{
		return StructProperty<AudioStreamBasicDescription>(SFBAudioObjectPropertyAddress(kAudioStreamPropertyVirtualFormat));
	}

	inline std::vector<AudioStreamRangedDescription> AvailableVirtualFormats() const
	{
		return ArrayProperty<AudioStreamRangedDescription>(SFBAudioObjectPropertyAddress(kAudioStreamPropertyAvailableVirtualFormats));
	}

	inline SFBAudioStreamBasicDescription PhysicalFormat() const
	{
		return StructProperty<AudioStreamBasicDescription>(SFBAudioObjectPropertyAddress(kAudioStreamPropertyPhysicalFormat));
	}

	inline std::vector<AudioStreamRangedDescription> AvailablePhysicalFormats() const
	{
		return ArrayProperty<AudioStreamRangedDescription>(SFBAudioObjectPropertyAddress(kAudioStreamPropertyAvailablePhysicalFormats));
	}

};
