/*
 * Copyright (c) 2014 - 2020 Stephen F. Booth <me@sbooth.org>
 * MIT license
 */

#import <algorithm>
#import <cstdlib>
#import <cstring>

#import "SFBRingBuffer.hpp"

namespace {

	/*!
	 * Return the smallest power of two value greater than @c x
	 * @param x A value in the range [2..2147483648]
	 * @return The smallest power of two greater than @c x
	 */
	inline constexpr uint32_t NextPowerOfTwo(uint32_t x) noexcept
	{
		assert(x > 1);
		assert(x <= ((std::numeric_limits<uint32_t>::max() / 2) + 1));
		return static_cast<uint32_t>(1 << (32 - __builtin_clz(x - 1)));
	}

}

#pragma mark Creation and Destruction

SFBRingBuffer::SFBRingBuffer() noexcept
: mBuffer(nullptr), mCapacityBytes(0), mCapacityBytesMask(0), mWritePosition(0), mReadPosition(0)
{
	assert(mWritePosition.is_lock_free());
}

SFBRingBuffer::~SFBRingBuffer()
{
	if(mBuffer)
		std::free(mBuffer);
}

#pragma mark Buffer Management

bool SFBRingBuffer::Allocate(size_t capacityBytes) noexcept
{
	if(capacityBytes < 2 || capacityBytes > 0x80000000)
		return false;

	Deallocate();

	// Round up to the next power of two
	capacityBytes = NextPowerOfTwo(static_cast<uint32_t>(capacityBytes));

	mBuffer = static_cast<uint8_t *>(std::malloc(capacityBytes));
	if(!mBuffer)
		return false;

	mCapacityBytes = capacityBytes;
	mCapacityBytesMask = capacityBytes - 1;

	return true;
}

void SFBRingBuffer::Deallocate() noexcept
{
	if(mBuffer) {
		std::free(mBuffer);
		mBuffer = nullptr;

		mCapacityBytes = 0;
		mCapacityBytesMask = 0;

		mReadPosition = 0;
		mWritePosition = 0;
	}
}

void SFBRingBuffer::Reset() noexcept
{
	mReadPosition = 0;
	mWritePosition = 0;
}

size_t SFBRingBuffer::BytesAvailableToRead() const noexcept
{
	auto writePosition = mWritePosition.load(std::memory_order_acquire);
	auto readPosition = mReadPosition.load(std::memory_order_acquire);

	if(writePosition > readPosition)
		return writePosition - readPosition;
	else
		return (writePosition - readPosition + mCapacityBytes) & mCapacityBytesMask;
}

size_t SFBRingBuffer::BytesAvailableToWrite() const noexcept
{
	auto writePosition = mWritePosition.load(std::memory_order_acquire);
	auto readPosition = mReadPosition.load(std::memory_order_acquire);

	if(writePosition > readPosition)
		return ((readPosition - writePosition + mCapacityBytes) & mCapacityBytesMask) - 1;
	else if(writePosition < readPosition)
		return (readPosition - writePosition) - 1;
	else
		return mCapacityBytes - 1;
}

#pragma mark Reading and Writing Data

size_t SFBRingBuffer::Read(void * const destinationBuffer, size_t byteCount) noexcept
{
	if(!destinationBuffer || 0 == byteCount)
		return 0;

	auto writePosition = mWritePosition.load(std::memory_order_acquire);
	auto readPosition = mReadPosition.load(std::memory_order_acquire);

	size_t bytesAvailable;
	if(writePosition > readPosition)
		bytesAvailable = writePosition - readPosition;
	else
		bytesAvailable = (writePosition - readPosition + mCapacityBytes) & mCapacityBytesMask;

	if(0 == bytesAvailable)
		return 0;

	size_t bytesToRead = std::min(bytesAvailable, byteCount);
	if(readPosition + bytesToRead > mCapacityBytes) {
		auto bytesAfterReadPointer = mCapacityBytes - readPosition;
		std::memcpy(destinationBuffer, mBuffer + readPosition, bytesAfterReadPointer);
		std::memcpy((uint8_t *)destinationBuffer + bytesAfterReadPointer, mBuffer, bytesToRead - bytesAfterReadPointer);
	}
	else
		std::memcpy(destinationBuffer, mBuffer + readPosition, bytesToRead);

	mReadPosition.store((readPosition + bytesToRead) & mCapacityBytesMask, std::memory_order_release);

	return bytesToRead;
}

size_t SFBRingBuffer::Peek(void * const destinationBuffer, size_t byteCount) const noexcept
{
	if(!destinationBuffer || 0 == byteCount)
		return 0;

	auto writePosition = mWritePosition.load(std::memory_order_acquire);
	auto readPosition = mReadPosition.load(std::memory_order_acquire);

	size_t bytesAvailable;
	if(writePosition > readPosition)
		bytesAvailable = writePosition - readPosition;
	else
		bytesAvailable = (writePosition - readPosition + mCapacityBytes) & mCapacityBytesMask;

	if(0 == bytesAvailable)
		return 0;

	size_t bytesToRead = std::min(bytesAvailable, byteCount);
	if(readPosition + bytesToRead > mCapacityBytes) {
		auto bytesAfterReadPointer = mCapacityBytes - readPosition;
		std::memcpy(destinationBuffer, mBuffer + readPosition, bytesAfterReadPointer);
		std::memcpy((uint8_t *)destinationBuffer + bytesAfterReadPointer, mBuffer, bytesToRead - bytesAfterReadPointer);
	}
	else
		std::memcpy(destinationBuffer, mBuffer + readPosition, bytesToRead);

	return bytesToRead;
}

size_t SFBRingBuffer::Write(const void * const sourceBuffer, size_t byteCount) noexcept
{
	if(!sourceBuffer || 0 == byteCount)
		return 0;

	auto writePosition = mWritePosition.load(std::memory_order_acquire);
	auto readPosition = mReadPosition.load(std::memory_order_acquire);

	size_t bytesAvailable;
	if(writePosition > readPosition)
		bytesAvailable = ((readPosition - writePosition + mCapacityBytes) & mCapacityBytesMask) - 1;
	else if(writePosition < readPosition)
		bytesAvailable = (readPosition - writePosition) - 1;
	else
		bytesAvailable = mCapacityBytes - 1;

	if(0 == bytesAvailable)
		return 0;

	size_t bytesToWrite = std::min(bytesAvailable, byteCount);
	if(writePosition + bytesToWrite > mCapacityBytes) {
		auto bytesAfterWritePointer = mCapacityBytes - writePosition;
		std::memcpy(mBuffer + writePosition, sourceBuffer, bytesAfterWritePointer);
		std::memcpy(mBuffer, (uint8_t *)sourceBuffer + bytesAfterWritePointer, bytesToWrite - bytesAfterWritePointer);
	}
	else
		std::memcpy(mBuffer + writePosition, sourceBuffer, bytesToWrite);

	mWritePosition.store((writePosition + bytesToWrite) & mCapacityBytesMask, std::memory_order_release);

	return bytesToWrite;
}

void SFBRingBuffer::AdvanceReadPosition(size_t byteCount) noexcept
{
	mReadPosition.store((mReadPosition.load(std::memory_order_acquire) + byteCount) & mCapacityBytesMask, std::memory_order_release);
}

void SFBRingBuffer::AdvanceWritePosition(size_t byteCount) noexcept
{
	mWritePosition.store((mWritePosition.load(std::memory_order_acquire) + byteCount) & mCapacityBytesMask, std::memory_order_release);
}

SFBRingBuffer::BufferPair SFBRingBuffer::ReadVector() const noexcept
{
	auto writePosition = mWritePosition.load(std::memory_order_acquire);
	auto readPosition = mReadPosition.load(std::memory_order_acquire);

	size_t bytesAvailable;
	if(writePosition > readPosition)
		bytesAvailable = writePosition - readPosition;
	else
		bytesAvailable = (writePosition - readPosition + mCapacityBytes) & mCapacityBytesMask;

	auto endOfRead = readPosition + bytesAvailable;

	if(endOfRead > mCapacityBytes)
		return { { mBuffer + readPosition, mCapacityBytes - readPosition }, { mBuffer, endOfRead & mCapacityBytes } };
	else
		return { { mBuffer + readPosition, bytesAvailable }, {} };
}

SFBRingBuffer::BufferPair SFBRingBuffer::WriteVector() const noexcept
{
	auto writePosition = mWritePosition.load(std::memory_order_acquire);
	auto readPosition = mReadPosition.load(std::memory_order_acquire);

	size_t bytesAvailable;
	if(writePosition > readPosition)
		bytesAvailable = ((readPosition - writePosition + mCapacityBytes) & mCapacityBytesMask) - 1;
	else if(writePosition < readPosition)
		bytesAvailable = (readPosition - writePosition) - 1;
	else
		bytesAvailable = mCapacityBytes - 1;

	auto endOfWrite = writePosition + bytesAvailable;

	if(endOfWrite > mCapacityBytes)
		return { { mBuffer + writePosition, mCapacityBytes - writePosition }, { mBuffer, endOfWrite & mCapacityBytes } };
	else
		return { { mBuffer + writePosition, bytesAvailable }, {} };
}
