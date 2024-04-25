//
// Copyright (c) 2014 - 2024 Stephen F. Booth <me@sbooth.org>
// Part of https://github.com/sbooth/SFBAudioUtilities
// MIT license
//

#pragma once

#import <atomic>
#import <optional>
#import <type_traits>
#import <utility>

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
	RingBuffer() noexcept = default;

	// This class is non-copyable
	RingBuffer(const RingBuffer& rhs) = delete;

	// This class is non-assignable
	RingBuffer& operator=(const RingBuffer& rhs) = delete;

	/// Destroys the @c RingBuffer and releases all associated resources.
	inline ~RingBuffer()
	{
		std::free(mBuffer);
	}

	// This class is non-movable
	RingBuffer(RingBuffer&& rhs) = delete;

	// This class is non-move assignable
	RingBuffer& operator=(RingBuffer&& rhs) = delete;

#pragma mark Buffer Management

	/// Allocates space for data.
	/// @attention This method is not thread safe.
	/// @note Capacities from 2 to 2,147,483,648 (0x80000000) bytes are supported
	/// @param byteCount The desired capacity, in bytes
	/// @return @c true on success, @c false on error
	bool Allocate(uint32_t byteCount) noexcept;

	/// Frees the resources used by this @c RingBuffer
	/// @attention This method is not thread safe.
	void Deallocate() noexcept;


	/// Resets this @c RingBuffer to its default state.
	/// @attention This method is not thread safe.
	void Reset() noexcept;

#pragma mark Buffer Information

	/// Returns the capacity of this RingBuffer in bytes
	inline uint32_t CapacityBytes() const noexcept
	{
		return mCapacityBytes;
	}

	/// Returns the number of bytes available for reading
	uint32_t BytesAvailableToRead() const noexcept;

	/// Returns the free space available for writing in bytes
	uint32_t BytesAvailableToWrite() const noexcept;

#pragma mark Reading and Writing Data

	/// Read data from the @c RingBuffer, advancing the read pointer.
	/// @param destinationBuffer An address to receive the data
	/// @param byteCount The desired number of bytes to read
	/// @param allowPartial Whether any bytes should be read if the number of bytes available for reading is less than @c byteCount
	/// @return The number of bytes actually read
	uint32_t Read(void * const _Nonnull destinationBuffer, uint32_t byteCount, bool allowPartial = true) noexcept;

	/// Read data from the @c RingBuffer without advancing the read pointer.
	/// @param destinationBuffer An address to receive the data
	/// @param byteCount The desired number of bytes to read
	/// @param allowPartial Whether any bytes should be read if the number of bytes available for reading is less than @c byteCount
	/// @return The number of bytes actually read
	uint32_t Peek(void * const _Nonnull destinationBuffer, uint32_t byteCount, bool allowPartial = true) const noexcept;

	/// Write data to the @c RingBuffer, advancing the write pointer.
	/// @param sourceBuffer An address containing the data to copy
	/// @param byteCount The desired number of bytes to write
	/// @param allowPartial Whether any bytes should be written if the free space available for writing is less than @c byteCount
	/// @return The number of bytes actually written
	uint32_t Write(const void * const _Nonnull sourceBuffer, uint32_t byteCount, bool allowPartial = true) noexcept;

#pragma mark Reading and Writing Types

	/// Read a type from the @c RingBuffer, advancing the read pointer.
	/// @tparam T A trivially copyable type to read
	/// @param value The destination value
	/// @return @c true on success, @c false otherwise
	template <typename T, typename = std::enable_if_t<std::is_trivially_copyable_v<T>>>
	bool ReadValue(T& value) noexcept
	{
		auto size = static_cast<uint32_t>(sizeof(T));
		auto bytesRead = Read(static_cast<void *>(&value), size, false);
		if(bytesRead != size)
			return false;
		return true;
	}

	/// Read a type from the @c RingBuffer, advancing the read pointer.
	/// @tparam T A trivially copyable and trivially default constructible type to read
	/// @return A @c std::optional containing an instance of @c T if sufficient bytes were available for reading
	template <typename T, typename = std::enable_if_t<std::is_trivially_copyable_v<T> && std::is_trivially_default_constructible_v<T>>>
	std::optional<T> ReadValue() noexcept
	{
		T value{};
		if(!ReadValue(value))
			return std::nullopt;
		return value;
	}

	/// Read a type from the @c RingBuffer without advancing the read pointer.
	/// @tparam T A trivially copyable type to read
	/// @param value The destination value
	/// @return @c true on success, @c false otherwise
	template <typename T, typename = std::enable_if_t<std::is_trivially_copyable_v<T>>>
	bool PeekValue(T& value) const noexcept
	{
		auto size = static_cast<uint32_t>(sizeof(T));
		auto bytesRead = Peek(static_cast<void *>(&value), size, false);
		if(bytesRead != size)
			return false;
		return true;
	}

	/// Read a type from the @c RingBuffer without advancing the read pointer.
	/// @tparam T A trivially copyable and trivially default constructible type to read
	/// @return A @c std::optional containing an instance of @c T if sufficient bytes were available for reading
	template <typename T, typename = std::enable_if_t<std::is_trivially_copyable_v<T> && std::is_trivially_default_constructible_v<T>>>
	std::optional<T> PeekValue() const noexcept
	{
		T value{};
		if(!PeekValue(value))
			return std::nullopt;
		return value;
	}

	/// Write a type to the @c RingBuffer, advancing the write pointer.
	/// @tparam T A trivially copyable type to write
	/// @param value The value to write
	/// @return @c true if @c value was successfully written
	template <typename T, typename = std::enable_if_t<std::is_trivially_copyable_v<T>>>
	bool WriteValue(const T& value) noexcept
	{
		auto size = static_cast<uint32_t>(sizeof(T));
		auto bytesWritten = Write(static_cast<const void *>(&value), size, false);
		if(bytesWritten != size)
			return false;
		return true;
	}

#pragma mark Advanced Reading and Writing

	/// Advance the read position by the specified number of bytes
	void AdvanceReadPosition(uint32_t byteCount) noexcept;

	/// Advance the write position by the specified number of bytes
	void AdvanceWritePosition(uint32_t byteCount) noexcept;


	/// A read-only memory buffer
	struct ReadBuffer {
		/// The memory buffer location
		const uint8_t * const _Nullable mBuffer = nullptr;
		/// The number of bytes of valid data in @c mBuffer
		const uint32_t mBufferSize = 0;

		/// Construct an empty @c ReadBuffer
		ReadBuffer() noexcept = default;

		/// Construct a @c ReadBuffer for the specified location and size
		/// @param buffer The memory buffer location
		/// @param bufferSize The number of bytes of valid data in @c buffer
		ReadBuffer(const uint8_t * const _Nullable buffer, uint32_t bufferSize) noexcept
		: mBuffer{buffer}, mBufferSize{bufferSize}
		{}
	};

	/// A pair of @c ReadBuffer objects
	using ReadBufferPair = std::pair<const ReadBuffer, const ReadBuffer>;

	/// Returns the read vector containing the current readable data
	const ReadBufferPair ReadVector() const noexcept;


	/// A write-only memory buffer
	struct WriteBuffer {
		/// The memory buffer location
		uint8_t * const _Nullable mBuffer = nullptr;
		/// The capacity of @c mBuffer in bytes
		const uint32_t mBufferCapacity = 0;

		/// Construct an empty @c WriteBuffer
		WriteBuffer() noexcept = default;

		/// Construct a @c WriteBuffer for the specified location and capacity
		/// @param buffer The memory buffer location
		/// @param bufferCapacity The capacity of @c buffer in bytes
		WriteBuffer(uint8_t * const _Nullable buffer, uint32_t bufferCapacity) noexcept
		: mBuffer{buffer}, mBufferCapacity{bufferCapacity}
		{}
	};

	/// A pair of @c WriteBuffer objects
	using WriteBufferPair = std::pair<const WriteBuffer, const WriteBuffer>;

	/// Returns the write vector containing the current writable space
	const WriteBufferPair WriteVector() const noexcept;

private:

	/// The memory buffer holding the data
	uint8_t * _Nullable mBuffer = nullptr;

	/// The capacity of @c mBuffer in bytes
	uint32_t mCapacityBytes = 0;
	/// The capacity of @c mBuffer in bytes minus one
	uint32_t mCapacityBytesMask = 0;

	/// The offset into @c mBuffer of the read location
	std::atomic_uint32_t mWritePosition = 0;
	/// The offset into @c mBuffer of the write location
	std::atomic_uint32_t mReadPosition = 0;

	static_assert(std::atomic_uint32_t::is_always_lock_free, "Lock-free std::atomic_uint32_t required");

};

} // namespace SFB
