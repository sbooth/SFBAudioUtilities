//
// Copyright (c) 2013 - 2024 Stephen F. Booth <me@sbooth.org>
// Part of https://github.com/sbooth/SFBAudioUtilities
// MIT license
//

#pragma once

#import <atomic>
#import <cstdlib>

#import <CoreAudioTypes/CoreAudioTypes.h>

#import "SFBCAStreamBasicDescription.hpp"

namespace SFB {

/// A ring buffer supporting non-interleaved audio.
///
/// This class is thread safe when used from one reader thread and one writer thread (single producer, single consumer model).
class AudioRingBuffer
{

public:

#pragma mark Creation and Destruction

	/// Creates a new @c AudioRingBuffer
	/// @note @c Allocate() must be called before the object may be used.
	constexpr AudioRingBuffer() noexcept = default;

	// This class is non-copyable
	AudioRingBuffer(const AudioRingBuffer& rhs) = delete;

	// This class is non-assignable
	AudioRingBuffer& operator=(const AudioRingBuffer& rhs) = delete;

	/// Destroys the @c AudioRingBuffer and releases all associated resources.
	inline ~AudioRingBuffer()
	{
		std::free(mBuffers);
	}

	// This class is non-movable
	AudioRingBuffer(AudioRingBuffer&& rhs) = delete;

	// This class is non-move assignable
	AudioRingBuffer& operator=(AudioRingBuffer&& rhs) = delete;

#pragma mark Buffer management

	/// Allocates space for audio data.
	/// @note Only interleaved formats are supported.
	/// @note This method is not thread safe.
	/// @note Capacities from 2 to 2,147,483,648 (0x80000000) frames are supported
	/// @param format The format of the audio that will be written to and read from this buffer.
	/// @param capacityFrames The desired capacity, in frames
	/// @return @c true on success, @c false on error
	bool Allocate(const CAStreamBasicDescription& format, uint32_t capacityFrames) noexcept;

	/// Frees the resources used by this @c AudioRingBuffer
	/// @note This method is not thread safe.
	void Deallocate() noexcept;


	/// Resets this @c AudioRingBuffer to its default state.
	/// @note This method is not thread safe.
	void Reset() noexcept;


	/// Returns the capacity in frames of this @c AudioRingBuffer
	inline uint32_t CapacityFrames() const noexcept
	{
		return mCapacityFrames;
	}

	/// Returns the format of this @c AudioRingBuffer
	inline const CAStreamBasicDescription& Format() const noexcept
	{
		return mFormat;
	}

	/// Returns the number of frames available for reading
	uint32_t FramesAvailableToRead() const noexcept;

	/// Returns the free space available for writing in frames
	uint32_t FramesAvailableToWrite() const noexcept;

#pragma mark Reading and writing audio

	/// Reads audio from the @c AudioRingBuffer and advances the read pointer.
	/// @param bufferList An @c AudioBufferList to receive the audio
	/// @param frameCount The desired number of frames to read
	/// @param allowPartial Whether any frames should be read if the number of frames available for reading is less than @c frameCount
	/// @return The number of frames actually read
	uint32_t Read(AudioBufferList * const _Nonnull bufferList, uint32_t frameCount, bool allowPartial = true) noexcept;

	/// Writes audio to the @c AudioRingBuffer and advances the write pointer.
	/// @param bufferList An @c AudioBufferList containing the audio to copy
	/// @param frameCount The desired number of frames to write
	/// @param allowPartial Whether any frames should be written if the free space available for writing is less than @c frameCount
	/// @return The number of frames actually written
	uint32_t Write(const AudioBufferList * const _Nonnull bufferList, uint32_t frameCount, bool allowPartial = true) noexcept;

private:

	/// The format of the audio
	CAStreamBasicDescription mFormat = {};

	/// The channel pointers and buffers allocated in one chunk of memory
	uint8_t * _Nonnull * _Nullable mBuffers = nullptr;

	/// The frame capacity per channel
	uint32_t mCapacityFrames = 0;
	/// Mask used to wrap read and write locations
	/// @note Equal to @c mCapacityFrames-1
	uint32_t mCapacityFramesMask = 0;

	/// The offset in frames of the write location
	std::atomic_uint32_t mWritePointer = 0;
	/// The offset in frames of the read location
	std::atomic_uint32_t mReadPointer = 0;

	static_assert(std::atomic_uint32_t::is_always_lock_free, "Lock-free std::atomic_uint32_t required");

};

} // namespace SFB
