/*
 * Copyright (c) 2013 - 2021 Stephen F. Booth <me@sbooth.org>
 * MIT license
 */

#pragma once

#import <atomic>

#import <CoreAudio/CoreAudioTypes.h>

#import "SFBAudioStreamBasicDescription.hpp"

/*!
 * A ring buffer supporting non-interleaved audio.
 *
 * This class is thread safe when used from one reader thread and one writer thread (single producer, single consumer model).
 *
 * The read and write routines were originally based on JACK's ringbuffer implementation.
 */
class SFBAudioRingBuffer
{

public:
	
#pragma mark Creation and Destruction

	/*!
	 * Create a new @c SFBAudioRingBuffer
	 * @note Allocate() must be called before the object may be used.
	 */
	SFBAudioRingBuffer() noexcept;

	/*! Destroy the @c SFBAudioRingBuffer and release all associated resources. */
	~SFBAudioRingBuffer();

	/*! @internal This class is non-copyable */
	SFBAudioRingBuffer(const SFBAudioRingBuffer& rhs) = delete;

	/*! @internal This class is non-assignable */
	SFBAudioRingBuffer& operator=(const SFBAudioRingBuffer& rhs) = delete;

#pragma mark Buffer management

	/*!
	 * Allocate space for audio data.
	 * @note Only interleaved formats are supported.
	 * @note This method is not thread safe.
	 * @note Capacities from 2 to 2,147,483,648 (0x80000000) frames are supported
	 * @param format The format of the audio that will be written to and read from this buffer.
	 * @param capacityFrames The desired capacity, in frames
	 * @return @c true on success, @c false on error
	 */
	bool Allocate(const SFBAudioStreamBasicDescription& format, size_t capacityFrames) noexcept;

	/*!
	 * Free the resources used by this @c SFBAudioRingBuffer
	 * @note This method is not thread safe.
	 */
	void Deallocate() noexcept;


	/*!
	 * Reset this @c SFBAudioRingBuffer to its default state.
	 * @note This method is not thread safe.
	 */
	void Reset() noexcept;


	/*! Returns the capacity of this @c SFBAudioRingBuffer in frames */
	inline size_t CapacityFrames() const noexcept
	{
		return mCapacityFrames;
	}

	/*! Returns the format of this @c SFBAudioRingBuffer */
	inline const SFBAudioStreamBasicDescription& Format() const noexcept
	{
		return mFormat;
	}

	/*! Returns the number of frames available for reading */
	size_t FramesAvailableToRead() const noexcept;

	/*! Returns the free space available for writing in frames */
	size_t FramesAvailableToWrite() const noexcept;

#pragma mark Reading and writing audio

	/*!
	 * Read audio from the @c SFBAudioRingBuffer, advancing the read pointer.
	 * @param bufferList An @c AudioBufferList to receive the audio
	 * @param frameCount The desired number of frames to read
	 * @return The number of frames actually read
	 */
	size_t Read(AudioBufferList * const bufferList, size_t frameCount) noexcept;

	/*!
	 * Write audio to the @c SFBAudioRingBuffer, advancing the write pointer.
	 * @param bufferList An @c AudioBufferList containing the audio to copy
	 * @param frameCount The desired number of frames to write
	 * @return The number of frames actually written
	 */
	size_t Write(const AudioBufferList * const bufferList, size_t frameCount) noexcept;

private:

	SFBAudioStreamBasicDescription		mFormat;				// The format of the audio

	uint8_t				**mBuffers;				// The channel pointers and buffers, allocated in one chunk of memory

	size_t				mCapacityFrames;		// Frame capacity per channel
	size_t				mCapacityFramesMask;

	std::atomic_size_t	mWritePointer;			// In frames
	std::atomic_size_t	mReadPointer;

};
