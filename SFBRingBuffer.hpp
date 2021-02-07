//
// Copyright (c) 2014 - 2021 Stephen F. Booth <me@sbooth.org>
// MIT license
//

#pragma once

#import <atomic>

namespace SFB {

/// A generic ring buffer.
///
/// This class is thread safe when used from one reader thread and one writer thread (single producer, single consumer model).
class RingBuffer
{

public:

#pragma mark Creation and Destruction

	/// Creates a new @c RingBuffer
	/// @note @c Allocate() must be called before the object may be used.
	RingBuffer() noexcept;

	// This class is non-copyable
	RingBuffer(const RingBuffer& rhs) = delete;

	// This class is non-assignable
	RingBuffer& operator=(const RingBuffer& rhs) = delete;

	/// Destroys the @c RingBuffer and release all associated resources.
	~RingBuffer();

	// This class is non-movable
	RingBuffer(RingBuffer&& rhs) = delete;

	// This class is non-move assignable
	RingBuffer& operator=(RingBuffer&& rhs) = delete;

#pragma mark Buffer management

	/// Allocates space for data.
	/// @note This method is not thread safe.
	/// @note Capacities from 2 to 2,147,483,648 (0x80000000) bytes are supported
	/// @param byteCount The desired capacity, in bytes
	/// @return @c true on success, @c false on error
	bool Allocate(size_t byteCount) noexcept;

	/// Frees the resources used by this @c RingBuffer
	/// @note This method is not thread safe.
	void Deallocate() noexcept;


	/// Resets this @c RingBuffer to its default state.
	/// @note This method is not thread safe.
	void Reset() noexcept;


	/// Returns the capacity of this RingBuffer in bytes
	inline size_t CapacityBytes() const noexcept
	{
		return mCapacityBytes;
	}

	/// Returns the number of bytes available for reading
	size_t BytesAvailableToRead() const noexcept;

	/// Returns the free space available for writing in bytes
	size_t BytesAvailableToWrite() const noexcept;

#pragma mark Reading and writing data

	/// Read data from the @c RingBuffer, advancing the read pointer.
	/// @param destinationBuffer An address to receive the data
	/// @param byteCount The desired number of bytes to read
	/// @return The number of bytes actually read
	size_t Read(void * const _Nonnull destinationBuffer, size_t byteCount) noexcept;

	/// Read data from the @c RingBuffer without advancing the read pointer.
	/// @param destinationBuffer An address to receive the data
	/// @param byteCount The desired number of bytes to read
	/// @return The number of bytes actually read
	size_t Peek(void * const _Nonnull destinationBuffer, size_t byteCount) const noexcept;

	/// Write data to the @c RingBuffer, advancing the write pointer.
	/// @param sourceBuffer An address containing the data to copy
	/// @param byteCount The desired number of frames to write
	/// @return The number of bytes actually written
	size_t Write(const void * const _Nonnull sourceBuffer, size_t byteCount) noexcept;


	/// Advance the read position by the specified number of bytes
	void AdvanceReadPosition(size_t byteCount) noexcept;

	/// Advance the write position by the specified number of bytes
	void AdvanceWritePosition(size_t byteCount) noexcept;


	/// A struct wrapping a memory buffer location and capacity
	struct Buffer {
		/// The memory buffer location
		uint8_t	* const _Nullable mBuffer;
		/// The capacity of @c mBuffer in bytes
		size_t mBufferCapacity;

		/// Construct an empty Buffer
		Buffer() noexcept
		: Buffer(nullptr, 0)
		{}

		/// Construct a Buffer for the specified location and capacity
		/// @param buffer The memory buffer location
		/// @param bufferCapacity The capacity of @c buffer in bytes
		Buffer(uint8_t * const _Nullable buffer, size_t bufferCapacity) noexcept
		: mBuffer(buffer), mBufferCapacity(bufferCapacity)
		{}
	};

	/// A pair of @c Buffer objects
	using BufferPair = std::pair<Buffer, Buffer>;

	/// Returns the read vector containing the current readable data
	BufferPair ReadVector() const noexcept;

	/// Returns the write vector containing the current writeable data
	BufferPair WriteVector() const noexcept;

private:

	/// The memory buffer holding the data
	uint8_t * _Nullable mBuffer;

	/// The capacity of @c mBuffer in bytes
	size_t mCapacityBytes;
	/// The capacity of @c mBuffer in bytes minus one
	size_t mCapacityBytesMask;

	/// The offset into @c mBuffer of the read location
	std::atomic_size_t mWritePosition;
	/// The offset into @c mBuffer of the write location
	std::atomic_size_t mReadPosition;

};

} // namespace SFB
