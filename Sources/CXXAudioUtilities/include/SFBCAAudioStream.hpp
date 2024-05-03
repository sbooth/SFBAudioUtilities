//
// Copyright (c) 2021 - 2024 Stephen F. Booth <me@sbooth.org>
// Part of https://github.com/sbooth/SFBAudioUtilities
// MIT license
//

#pragma once

#import "SFBCAAudioObject.hpp"
#import "SFBCAStreamBasicDescription.hpp"

namespace SFB {

class CAAudioStream : public CAAudioObject
{

public:

	/// Creates an unknown @c CAAudioStream
	CAAudioStream() noexcept = default;

	/// Copy constructor
	CAAudioStream(const CAAudioStream& rhs) noexcept = default;

	/// Assignment operator
	CAAudioStream& operator=(const CAAudioStream& rhs) noexcept = default;

	/// Destructor
	virtual ~CAAudioStream() = default;

	/// Move constructor
	CAAudioStream(CAAudioStream&& rhs) = default;

	/// Move assignment operator
	CAAudioStream& operator=(CAAudioStream&& rhs) = default;


	/// Creates a @c CAAudioStream with the specified objectID
	inline CAAudioStream(AudioObjectID objectID) noexcept
	: CAAudioObject{objectID}
	{}

	inline bool IsActive() const
	{
		return ArithmeticProperty<UInt32>(CAPropertyAddress(kAudioStreamPropertyIsActive)) != 0;
	}

	inline UInt32 Direction() const
	{
		return ArithmeticProperty<UInt32>(CAPropertyAddress(kAudioStreamPropertyDirection));
	}

	inline UInt32 TerminalType() const
	{
		return ArithmeticProperty<UInt32>(CAPropertyAddress(kAudioStreamPropertyTerminalType));
	}

	inline UInt32 StartingChannel() const
	{
		return ArithmeticProperty<UInt32>(CAPropertyAddress(kAudioStreamPropertyStartingChannel));
	}

	inline UInt32 Latency() const
	{
		return ArithmeticProperty<UInt32>(CAPropertyAddress(kAudioStreamPropertyLatency));
	}

	inline CAStreamBasicDescription VirtualFormat() const
	{
		return StructProperty<AudioStreamBasicDescription>(CAPropertyAddress(kAudioStreamPropertyVirtualFormat));
	}

	inline std::vector<AudioStreamRangedDescription> AvailableVirtualFormats() const
	{
		return ArrayProperty<AudioStreamRangedDescription>(CAPropertyAddress(kAudioStreamPropertyAvailableVirtualFormats));
	}

	inline CAStreamBasicDescription PhysicalFormat() const
	{
		return StructProperty<AudioStreamBasicDescription>(CAPropertyAddress(kAudioStreamPropertyPhysicalFormat));
	}

	inline std::vector<AudioStreamRangedDescription> AvailablePhysicalFormats() const
	{
		return ArrayProperty<AudioStreamRangedDescription>(CAPropertyAddress(kAudioStreamPropertyAvailablePhysicalFormats));
	}

};

} // namespace SFB
