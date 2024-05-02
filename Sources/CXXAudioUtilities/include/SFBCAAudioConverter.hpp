//
// Copyright (c) 2024 Stephen F. Booth <me@sbooth.org>
// Part of https://github.com/sbooth/SFBAudioUtilities
// MIT license
//

#pragma once

#import <AudioToolbox/AudioConverter.h>

#import "SFBCAException.hpp"

CF_ASSUME_NONNULL_BEGIN

namespace SFB {

/// A wrapper around @c AudioConverter
class CAAudioConverter
{

public:
	/// Creates a @c CAAudioConverter
	constexpr CAAudioConverter() noexcept = default;

	// This class is non-copyable
	CAAudioConverter(const CAAudioConverter& rhs) = delete;

	// This class is non-assignable
	CAAudioConverter& operator=(const CAAudioConverter& rhs) = delete;

	/// Destroys the @c CAAudioConverter and release all associated resources.
	inline ~CAAudioConverter()
	{
		if(mConverter)
			AudioConverterDispose(mConverter);
	}

	/// Move constructor
	CAAudioConverter(CAAudioConverter&& rhs) noexcept
	: mConverter{rhs.mConverter}
	{
		rhs.mConverter = nullptr;
	}

	/// Move assignment operator
	CAAudioConverter& operator=(CAAudioConverter&& rhs) noexcept
	{
		if(this != &rhs) {
			mConverter = rhs.mConverter;
			rhs.mConverter = nullptr;
		}
		return *this;
	}

	/// Returns @c true if this object's internal @c AudioConverterRef is not @c nullptr
	inline explicit operator bool() const noexcept
	{
		return mConverter != nullptr;
	}

	/// Returns @c true if this object's internal @c AudioConverterRef is @c nullptr
	inline bool operator!() const noexcept
	{
		return !operator bool();
	}

	/// Returns @c true if this object's internal @c AudioConverterRef is not @c nullptr
	inline bool IsValid() const noexcept
	{
		return operator bool();
	}

	/// Returns the object's internal @c AudioConverterRef
	inline operator AudioConverterRef const _Nullable () const noexcept
	{
		return mConverter;
	}

	/// Creates a new audio converter.
	/// @throw @c std::system_error
	void New(const AudioStreamBasicDescription& inSourceFormat, const AudioStreamBasicDescription& inDestinationFormat)
	{
		Dispose();
		auto result = AudioConverterNew(&inSourceFormat, &inDestinationFormat, &mConverter);
		ThrowIfCAAudioConverterError(result, "AudioConverterNew");
	}

	/// Creates a new AudioConverter using specific codecs.
	/// @throw @c std::system_error
	void NewSpecific(const AudioStreamBasicDescription& inSourceFormat, const AudioStreamBasicDescription& inDestinationFormat, UInt32 inNumberClassDescriptions, const AudioClassDescription *inClassDescriptions)
	{
		Dispose();
		auto result = AudioConverterNewSpecific(&inSourceFormat, &inDestinationFormat, inNumberClassDescriptions, inClassDescriptions, &mConverter);
		ThrowIfCAAudioConverterError(result, "AudioConverterNewSpecific");
	}

	/// Destroys an existing audio converter.
	/// @throw @c std::system_error
	void Dispose()
	{
		if(mConverter) {
			auto result = AudioConverterDispose(mConverter);
			ThrowIfCAAudioConverterError(result, "AudioConverterDispose");
			mConverter = nullptr;
		}
	}

	/// Resets the audio converter.
	/// @throw @c std::system_error
	void Reset()
	{
		auto result = AudioConverterReset(mConverter);
		ThrowIfCAAudioConverterError(result, "AudioConverterReset");
	}

	/// Returns information about an audio converter property.
	/// @throw @c std::system_error
	void GetPropertyInfo(AudioConverterPropertyID inPropertyID, UInt32 * __nullable outSize, Boolean * __nullable outWritable)
	{
		auto result = AudioConverterGetPropertyInfo(mConverter, inPropertyID, outSize, outWritable);
		ThrowIfCAAudioConverterError(result, "AudioConverterGetPropertyInfo");
	}

	/// Returns an audio converter property value.
	/// @throw @c std::system_error
	void GetProperty(AudioConverterPropertyID inPropertyID, UInt32& ioPropertyDataSize, void *outPropertyData)
	{
		auto result = AudioConverterGetProperty(mConverter, inPropertyID, &ioPropertyDataSize, outPropertyData);
		ThrowIfCAAudioConverterError(result, "AudioConverterGetProperty");
	}

	/// Sets an audio converter property value.
	/// @throw @c std::system_error
	void SetProperty(AudioConverterPropertyID inPropertyID, UInt32 inPropertyDataSize, const void *inPropertyData)
	{
		auto result = AudioConverterSetProperty(mConverter, inPropertyID, inPropertyDataSize, inPropertyData);
		ThrowIfCAAudioConverterError(result, "AudioConverterSetProperty");
	}

	/// Converts data from an input buffer to an output buffer.
	/// @throw @c std::system_error
	void ConvertBuffer(UInt32 inInputDataSize, const void *inInputData, UInt32& ioOutputDataSize, void *outOutputData)
	{
		auto result = AudioConverterConvertBuffer(mConverter, inInputDataSize, inInputData, &ioOutputDataSize, outOutputData);
		ThrowIfCAAudioConverterError(result, "AudioConverterConvertBuffer");
	}

	/// Converts data supplied by an input callback function, supporting non-interleaved and packetized formats.
	/// @throw @c std::system_error
	void FillComplexBuffer(AudioConverterComplexInputDataProc inInputDataProc, void * __nullable inInputDataProcUserData, UInt32& ioOutputDataPacketSize, AudioBufferList *outOutputData, AudioStreamPacketDescription * __nullable outPacketDescription)
	{
		auto result = AudioConverterFillComplexBuffer(mConverter, inInputDataProc, inInputDataProcUserData, &ioOutputDataPacketSize, outOutputData, outPacketDescription);
		ThrowIfCAAudioConverterError(result, "AudioConverterFillComplexBuffer");
	}

	/// Converts PCM data from an input buffer list to an output buffer list.
	/// @throw @c std::system_error
	void ConvertComplexBuffer(UInt32 inNumberPCMFrames, const AudioBufferList *inInputData, AudioBufferList *outOutputData)
	{
		auto result = AudioConverterConvertComplexBuffer(mConverter, inNumberPCMFrames, inInputData, outOutputData);
		ThrowIfCAAudioConverterError(result, "AudioConverterConvertComplexBuffer");
	}

private:

	/// The underlying @c AudioConverter object
	AudioConverterRef _Nullable mConverter = nullptr;

};

} // namespace SFB

CF_ASSUME_NONNULL_END
