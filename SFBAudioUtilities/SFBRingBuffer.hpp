/*
 * Copyright (c) 2014 - 2020 Stephen F. Booth <me@sbooth.org>
 * MIT license
 */

#pragma once

#import <atomic>

/*!
 * @brief A ring buffer.
 *
 * This class is thread safe when used from one reader thread and one writer thread (single producer, single consumer model).
 *
 * The read and write routines were originally based on JACK's ringbuffer implementation.
 */
class SFBRingBuffer
{

public:

	// ========================================
	/*! @name Creation and Destruction */
	//@{

	/*!
	 * @brief Creates a new @c SFBRingBuffer
	 * @note @c Allocate() must be called before the object may be used.
	 */
	SFBRingBuffer() noexcept;

	/*! @brief Destroy the @c SFBRingBuffer and release all associated resources. */
	~SFBRingBuffer();

	/*! @cond */

	/*! @internal This class is non-copyable */
	SFBRingBuffer(const SFBRingBuffer& rhs) = delete;

	/*! @internal This class is non-assignable */
	SFBRingBuffer& operator=(const SFBRingBuffer& rhs) = delete;

	/*! @endcond */

	//@}


	// ========================================
	/*! @name Buffer management */
	//@{

	/*!
	 * @brief Allocate space for data.
	 * @note This method is not thread safe.
	 * @note Capacities from 2 to 2,147,483,648 (0x80000000) bytes are supported
	 * @param byteCount The desired capacity, in bytes
	 * @return @c true on success, @c false on error
	 */
	bool Allocate(size_t byteCount) noexcept;

	/*!
	 * @brief Free the resources used by this @c SFBRingBuffer
	 * @note This method is not thread safe.
	 */
	void Deallocate() noexcept;


	/*!
	 * @brief Reset this @c SFBRingBuffer to its default state.
	 * @note This method is not thread safe.
	 */
	void Reset() noexcept;


	/*! @brief Returns the capacity of this SFBRingBuffer in bytes */
	inline size_t CapacityBytes() const noexcept
	{
		return mCapacityBytes;		
	}

	/*! @brief Returns the number of bytes available for reading */
	size_t BytesAvailableToRead() const noexcept;

	/*! @brief Returns the free space available for writing in bytes */
	size_t BytesAvailableToWrite() const noexcept;

	//@}


	// ========================================
	/*! @name Reading and writing data */
	//@{

	/*!
	 * @brief Read data from the @c SFBRingBuffer, advancing the read pointer.
	 * @param destinationBuffer An address to receive the data
	 * @param byteCount The desired number of bytes to read
	 * @return The number of bytes actually read
	 */
	size_t Read(void * const destinationBuffer, size_t byteCount) noexcept;

	/*!
	 * @brief Read data from the @c SFBRingBuffer without advancing the read pointer.
	 * @param destinationBuffer An address to receive the data
	 * @param byteCount The desired number of bytes to read
	 * @return The number of bytes actually read
	 */
	size_t Peek(void * const destinationBuffer, size_t byteCount) const noexcept;

	/*!
	 * @brief Write data to the @c SFBRingBuffer, advancing the write pointer.
	 * @param sourceBuffer An address containing the data to copy
	 * @param byteCount The desired number of frames to write
	 * @return The number of bytes actually written
	 */
	size_t Write(const void * const sourceBuffer, size_t byteCount) noexcept;


	/*! @brief Advance the read position by the specified number of bytes */
	void AdvanceReadPosition(size_t byteCount) noexcept;

	/*! @brief Advance the write position by the specified number of bytes */
	void AdvanceWritePosition(size_t byteCount) noexcept;


	/*! @brief A struct wrapping a memory buffer location and capacity */
	struct Buffer {
		uint8_t	* const mBuffer;	/*!< The memory buffer location */
		size_t	mBufferCapacity;	/*!< The capacity of @c mBuffer in bytes */

		/*! @brief Construct an empty Buffer */
		Buffer() noexcept
		: Buffer(nullptr, 0)
		{}

		/*!
		 * @brief Construct a Buffer for the specified location and capacity
		 * @param buffer The memory buffer location
		 * @param bufferCapacity The capacity of @c buffer in bytes
		 */
		Buffer(uint8_t * const buffer, size_t bufferCapacity) noexcept
		: mBuffer(buffer), mBufferCapacity(bufferCapacity)
		{}
	};

	/*! @brief A pair of @c Buffer objects */
	using BufferPair = std::pair<Buffer, Buffer>;

	/*! @brief Returns the read vector containing the current readable data */
	BufferPair ReadVector() const noexcept;

	/*! @brief Returns the write vector containing the current writeable data */
	BufferPair WriteVector() const noexcept;

	//@}

private:

	uint8_t				*mBuffer;				/*!< The memory buffer holding the data */

	size_t				mCapacityBytes;			/*!< The capacity of @c mBuffer in bytes */
	size_t				mCapacityBytesMask;		/*!< The capacity of @c mBuffer in bytes minus one */

	std::atomic_size_t	mWritePosition;			/*!< The offset into @c mBuffer of the read location */
	std::atomic_size_t	mReadPosition;			/*!< The offset into @c mBuffer of the write location */
};
