//
// Copyright (c) 2014 - 2021 Stephen F. Booth <me@sbooth.org>
// MIT license
//

#pragma once

#import <cstring>

#import <CoreAudio/CoreAudioTypes.h>

#import "SFBCFWrapper.hpp"

/// Common PCM audio formats
typedef CF_ENUM(uint32_t, SFBCommonPCMFormat) {
	/// Native-endian @c float
	kSFBCommonPCMFormatFloat32 			= 1,
	/// Native-endian @c double
	kSFBCommonPCMFormatFloat64 			= 2,
	/// Native-endian @c int16_t
	kSFBCommonPCMFormatInt16 			= 3,
	/// Native-endian @c int32_t
	kSFBCommonPCMFormatInt32 			= 4,
};

/// A class extending the functionality of a Core Audio @c AudioStreamBasicDescription
class SFBAudioStreamBasicDescription : public AudioStreamBasicDescription
{

public:
	
#pragma mark Creation and Destruction

	/// Creates an empty @c SFBAudioStreamBasicDescription
	inline SFBAudioStreamBasicDescription() noexcept
	{
		std::memset(this, 0, sizeof(AudioStreamBasicDescription));
	}

	/// Creates a new @c SFBAudioStreamBasicDescription for the specified @c AudioStreamBasicDescription
	inline SFBAudioStreamBasicDescription(const AudioStreamBasicDescription& asbd) noexcept
	{
		std::memcpy(this, &asbd, sizeof(AudioStreamBasicDescription));
	}

	/// Creates a new @c SFBAudioStreamBasicDescription for the specified @c AudioStreamBasicDescription
	inline SFBAudioStreamBasicDescription(const AudioStreamBasicDescription * const asbd) noexcept
	{
		assert(asbd != nullptr);
		std::memcpy(this, asbd, sizeof(AudioStreamBasicDescription));
	}

	/// Creates a new @c SFBAudioStreamBasicDescription for the speciifed @c SFBCommonPCMFormat
	SFBAudioStreamBasicDescription(SFBCommonPCMFormat commonPCMFormat, Float32 sampleRate, UInt32 channelsPerFrame, bool isInterleaved) noexcept;

	/// Copy constructor
	inline SFBAudioStreamBasicDescription(const SFBAudioStreamBasicDescription& rhs) noexcept
	{
		*this = rhs;
	}

	/// Assignment operator
	inline SFBAudioStreamBasicDescription& operator=(const AudioStreamBasicDescription& rhs) noexcept
	{
		if(this != &rhs)
			std::memcpy(this, &rhs, sizeof(AudioStreamBasicDescription));
		return *this;
	}

	/// Returns @c true if @c rhs is equal to @c this
	inline bool operator==(const AudioStreamBasicDescription& rhs) const noexcept
	{
		return !std::memcmp(this, &rhs, sizeof(AudioStreamBasicDescription));
	}

	/// Returns @c true if @c rhs is not equal to @c this
	inline bool operator!=(const AudioStreamBasicDescription& rhs) const noexcept
	{
		return !operator==(rhs);
	}

#pragma mark Format information

	/// Returns @c true if this format is interleaved
	inline bool IsInterleaved() const noexcept
	{
		return (mFormatFlags & kAudioFormatFlagIsNonInterleaved) == 0;
	}

	/// Returns the number of interleaved channels
	inline UInt32 InterleavedChannelCount() const noexcept
	{
		return IsInterleaved() ? mChannelsPerFrame : 1;
	}

	/// Returns the number of channel streams
	inline UInt32 ChannelStreamCount() const noexcept
	{
		return IsInterleaved() ? 1 : mChannelsPerFrame;
	}

	/// Returns the number of channels
	inline UInt32 ChannelCount() const noexcept
	{
		return mChannelsPerFrame;
	}

	/// Returns @c true if this format is PCM
	inline bool IsPCM() const noexcept
	{
		return kAudioFormatLinearPCM == mFormatID;
	}

	/// Returns @c true if this format is big-endian
	inline bool IsBigEndian() const noexcept
	{
		return (mFormatFlags & kAudioFormatFlagIsBigEndian) == kAudioFormatFlagIsBigEndian;
	}

	/// Returns @c true if this format is little-endian
	inline bool IsLittleEndian() const noexcept
	{
		return !IsBigEndian();
	}

	/// Returns @c true if this format is native-endian
	inline bool IsNativeEndian() const noexcept
	{
		return (mFormatFlags & kAudioFormatFlagIsBigEndian) == kAudioFormatFlagsNativeEndian;
	}

	/// Returns @c true if this format is floating-point linear PCM
	inline bool IsFloat() const noexcept
	{
		return IsPCM() && (mFormatFlags & kAudioFormatFlagIsFloat) == kAudioFormatFlagIsFloat;
	}

	/// Returns @c true if this format is signed integer linear PCM
	inline bool IsSignedInteger() const noexcept
	{
		return IsPCM() && (mFormatFlags & kAudioFormatFlagIsSignedInteger) == kAudioFormatFlagIsSignedInteger;
	}

	/// Returns @c true if this format is packed
	inline bool IsPacked() const noexcept
	{
		return (mFormatFlags & kAudioFormatFlagIsPacked) == kAudioFormatFlagIsPacked;
	}

	/// Returns @c true if this format is high-aligned
	inline bool IsAlignedHigh() const noexcept
	{
		return (mFormatFlags & kAudioFormatFlagIsAlignedHigh) == kAudioFormatFlagIsAlignedHigh;
	}

	/// Returns @c true if this format is non-mixable
	/// @note This flag is only used when interacting with HAL stream formats
	inline bool IsNonMixable() const noexcept
	{
		return (mFormatFlags & kAudioFormatFlagIsNonMixable) == kAudioFormatFlagIsNonMixable;
	}

	/// Returns @c true if this format is mixable
	/// @note This flag is only used when interacting with HAL stream formats
	inline bool IsMixable() const noexcept
	{
		return IsPCM() && !IsNonMixable();
	}

	/// Returns the sample word size in bytes
	inline UInt32 SampleWordSize() const noexcept
	{
		auto interleavedChannelCount = InterleavedChannelCount();
		if(!interleavedChannelCount)
//			throw std::runtime_error("InterleavedChannelCount() == 0 in SampleWordSize()");
			return 0;
		return mBytesPerFrame / interleavedChannelCount;
	}

	/// Returns the byte size of @c frameCount audio frames
	/// @note This is equivalent to @c frameCount*mBytesPerFrame
	inline UInt32 FrameCountToByteSize(UInt32 frameCount) const noexcept
	{
		return frameCount * mBytesPerFrame;
	}

	/// Returns the frame count of @c byteSize bytes
	/// @note This is equivalent to @c byteSize/mBytesPerFrame
	inline UInt32 ByteSizeToFrameCount(UInt32 byteSize) const
	{
		if(!mBytesPerFrame)
//			throw std::runtime_error("mBytesPerFrame == 0 in ByteSizeToFrameCount()");
			return 0;
		return byteSize / mBytesPerFrame;
	}

#pragma mark Format transformation

	/// Sets @c format to the equivalent non-interleaved format of @c this. Fails for non-PCM formats.
	bool GetNonInterleavedEquivalent(SFBAudioStreamBasicDescription& format) const noexcept;

	/// Sets @c format to the equivalent interleaved format of @c this. Fails for non-PCM formats.
	bool GetInterleavedEquivalent(SFBAudioStreamBasicDescription& format) const noexcept;

	/// Sets @c format to the equivalent standard format of @c this. Fails for non-PCM formats.
	bool GetStandardEquivalent(SFBAudioStreamBasicDescription& format) const noexcept;

	/// Resets the @c SFBAudioStreamBasicDescription to the default state
	inline void Reset() noexcept
	{
		std::memset(this, 0, sizeof(AudioStreamBasicDescription));
	}


	/// Returns a string representation of this format suitable for logging
	SFBCFString Description(const char * const prefix = nullptr) const noexcept;

};
