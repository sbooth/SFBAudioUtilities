/*
 * Copyright (c) 2014 - 2021 Stephen F. Booth <me@sbooth.org>
 * MIT license
 */

#pragma once

#import <CoreAudio/CoreAudioTypes.h>

#import "SFBCFWrapper.hpp"

/*! @brief Common PCM audio formats */
typedef CF_ENUM(uint32_t, SFBCommonPCMFormat) {
	kSFBCommonPCMFormatFloat32 			= 1, 		/*!< Native-endian \c float */
	kSFBCommonPCMFormatFloat64 			= 2, 		/*!< Native-endian \c double */
	kSFBCommonPCMFormatInt16 			= 3, 		/*!< Native-endian signed 16-bit integers */
	kSFBCommonPCMFormatInt32 			= 4, 		/*!< Native-endian signed 32-bit integers */
};

/*! @brief A class extending the functionality of a Core %Audio \c AudioStreamBasicDescription */
class SFBAudioStreamBasicDescription : public AudioStreamBasicDescription
{

public:
	
	// ========================================
	/*! @name Creation and Destruction */
	//@{

	/*! @brief Create a new, empty \c SFBAudioStreamBasicDescription */
	inline SFBAudioStreamBasicDescription() noexcept
	{
		memset(this, 0, sizeof(AudioStreamBasicDescription));
	}

	/*! @brief Create a new \c SFBAudioStreamBasicDescription for the specified \c AudioStreamBasicDescription */
	inline SFBAudioStreamBasicDescription(const AudioStreamBasicDescription& asbd) noexcept
	{
		memcpy(this, &asbd, sizeof(AudioStreamBasicDescription));
	}

	/*! @brief Create a new \c SFBAudioStreamBasicDescription for the specified \c AudioStreamBasicDescription */
	inline SFBAudioStreamBasicDescription(const AudioStreamBasicDescription * const asbd) noexcept
	{
		assert(asbd != nullptr);
		memcpy(this, asbd, sizeof(AudioStreamBasicDescription));
	}

	/*! @brief Create a new \c SFBAudioStreamBasicDescription for the speciifed \c SFBCommonPCMFormat */
	SFBAudioStreamBasicDescription(SFBCommonPCMFormat format, Float32 sampleRate, UInt32 channelsPerFrame, bool isInterleaved) noexcept;

	/*! @brief Copy constructor */
	inline SFBAudioStreamBasicDescription(const SFBAudioStreamBasicDescription& rhs) noexcept
	{
		*this = rhs;
	}

	/*! @brief Assignment operator */
	inline SFBAudioStreamBasicDescription& operator=(const SFBAudioStreamBasicDescription& rhs) noexcept
	{
		memcpy(this, &rhs, sizeof(AudioStreamBasicDescription));
		return *this;
	}

	/*! @brief Compare two \c SFBAudioStreamBasicDescription objects for equality*/
	inline bool operator==(const SFBAudioStreamBasicDescription& rhs) const noexcept
	{
		return !memcmp(this, &rhs, sizeof(AudioStreamBasicDescription));
	}

	/*! @brief Compare two \c SFBAudioStreamBasicDescription objects for inequality*/
	inline bool operator!=(const SFBAudioStreamBasicDescription& rhs) const noexcept
	{
		return !operator==(rhs);
	}

	//@}


	// ========================================
	/*! @name Format information */
	//@{

	/*! @brief Query whether this format represents interleaved data */
	inline bool IsInterleaved() const noexcept
	{
		return (mFormatFlags & kAudioFormatFlagIsNonInterleaved) == 0;
	}

	/*! @brief Returns the number of interleaved channels */
	inline UInt32 InterleavedChannelCount() const noexcept
	{
		return IsInterleaved() ? mChannelsPerFrame : 1;
	}

	/*! @brief Returns the number of channel streams */
	inline UInt32 ChannelStreamCount() const noexcept
	{
		return IsInterleaved() ? 1 : mChannelsPerFrame;
	}

	/*! @brief Returns the number of channels */
	inline UInt32 ChannelCount() const noexcept
	{
		return mChannelsPerFrame;
	}

	/*! @brief Query whether this format represents PCM audio data */
	inline bool IsPCM() const noexcept
	{
		return kAudioFormatLinearPCM == mFormatID;
	}

	/*! @brief Query whether this format represents big-endian ordered data */
	inline bool IsBigEndian() const noexcept
	{
		return (mFormatFlags & kAudioFormatFlagIsBigEndian) == kAudioFormatFlagIsBigEndian;
	}

	/*! @brief Query whether this format represents little-endian ordered data */
	inline bool IsLittleEndian() const noexcept
	{
		return !IsBigEndian();
	}

	/*! @brief Query whether this format represents native-endian ordered data */
	inline bool IsNativeEndian() const noexcept
	{
		return (mFormatFlags & kAudioFormatFlagIsBigEndian) == kAudioFormatFlagsNativeEndian;
	}

	/*! @brief Query whether this format represents floating-point data */
	inline bool IsFloat() const noexcept
	{
		return (mFormatFlags & kAudioFormatFlagIsFloat) == kAudioFormatFlagIsFloat;
	}

	/*! @brief Query whether this format represents signed integer data */
	inline bool IsSignedInteger() const noexcept
	{
		return (mFormatFlags & kAudioFormatFlagIsSignedInteger) == kAudioFormatFlagIsSignedInteger;
	}

	/*! @brief Query whether this format represents packed data */
	inline bool IsPacked() const noexcept
	{
		return (mFormatFlags & kAudioFormatFlagIsPacked) == kAudioFormatFlagIsPacked;
	}


	/*! @brief Query whether this format is high-aligned */
	inline bool IsAlignedHigh() const noexcept
	{
		return (mFormatFlags & kAudioFormatFlagIsAlignedHigh) == kAudioFormatFlagIsAlignedHigh;
	}

	//@}


	// ========================================
	/*! @name Format transformation */
	//@{

	/*! @brief Sets \c format to the equivalent non-interleaved format of \c this. Fails for non-PCM formats. */
	bool GetNonInterleavedEquivalent(SFBAudioStreamBasicDescription& format) const noexcept;

	/*! @brief Sets \c format to the equivalent interleaved format of \c this. Fails for non-PCM formats. */
	bool GetInterleavedEquivalent(SFBAudioStreamBasicDescription& format) const noexcept;

	/*! @brief Sets \c format to the equivalent standard format of \c this. Fails for non-PCM formats. */
	bool GetStandardEquivalent(SFBAudioStreamBasicDescription& format) const noexcept;

	//@}


	/*! @brief Returns a string representation of this format suitable for logging */
	SFBCFString Description(const char * const prefix = nullptr) const noexcept;

};
