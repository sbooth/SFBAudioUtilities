/*
 * Copyright (c) 2013 - 2021 Stephen F. Booth <me@sbooth.org>
 * MIT license
 */

#import <algorithm>
#import <cstdlib>
#import <limits>

#import "SFBCARingBuffer.hpp"

namespace {

	/*! @brief Returns \c v clamped to the interval \c [lo,hi] */
	template<typename T>
	constexpr const T& clamp(const T& v, const T& lo, const T& hi)
	{
		assert(!(hi < lo));
		return (v < lo) ? lo : (hi < v) ? hi : v;
	}

	/*!
	 * Zeroes a range of bytes in \c buffers
	 * @param buffers The destination buffers
	 * @param bufferCount The number of buffers
	 * @param byteOffset The byte offset in \c buffers to begin writing
	 * @param byteCount The number of bytes per non-interleaved buffer to write
	 */
	inline void ZeroRange(uint8_t * const * const buffers, size_t bufferCount, size_t byteOffset, size_t byteCount)
	{
		for(auto bufferIndex = 0; bufferIndex < bufferCount; ++bufferIndex)
			memset(buffers[bufferIndex] + byteOffset, 0, byteCount);
	}

	/*!
	 * Zeroes a range of bytes in \c bufferList
	 * @param bufferList The destination buffers
	 * @param byteOffset The byte offset in \c bufferList to begin writing
	 * @param byteCount The maximum number of bytes per non-interleaved buffer to write
	 */
	inline void ZeroABL(AudioBufferList * const bufferList, size_t byteOffset, size_t byteCount)
	{
		for(auto bufferIndex = 0; bufferIndex < bufferList->mNumberBuffers; ++bufferIndex) {
			if(byteOffset > bufferList->mBuffers[bufferIndex].mDataByteSize)
				continue;
			memset(static_cast<uint8_t *>(bufferList->mBuffers[bufferIndex].mData) + byteOffset, 0, std::min(byteCount, bufferList->mBuffers[bufferIndex].mDataByteSize - byteOffset));
		}
	}

	/*!
	 * Copies non-interleaved audio from \c bufferList to \c buffers
	 * @param buffers The destination buffers
	 * @param dstOffset The byte offset in \c buffers to begin writing
	 * @param bufferList The source buffers
	 * @param srcOffset The byte offset in \c bufferList to begin reading
	 * @param byteCount The maximum number of bytes per non-interleaved buffer to read and write
	 */
	inline void StoreABL(uint8_t * const * const buffers, size_t dstOffset, const AudioBufferList * const bufferList, size_t srcOffset, size_t byteCount) noexcept
	{
		for(auto bufferIndex = 0; bufferIndex < bufferList->mNumberBuffers; ++bufferIndex) {
			if(srcOffset > bufferList->mBuffers[bufferIndex].mDataByteSize)
				continue;
			memcpy(buffers[bufferIndex] + dstOffset, static_cast<uint8_t *>(bufferList->mBuffers[bufferIndex].mData) + srcOffset, std::min(byteCount, bufferList->mBuffers[bufferIndex].mDataByteSize - srcOffset));
		}
	}

	/*!
	 * Copies non-interleaved audio from \c buffers to \c bufferList
	 * @param bufferList The destination buffers
	 * @param dstOffset The byte offset in \c bufferList to begin writing
	 * @param buffers The source buffers
	 * @param srcOffset The byte offset in \c bufferList to begin reading
	 * @param byteCount The maximum number of bytes per non-interleaved buffer to read and write
	 */
	inline void FetchABL(AudioBufferList * const bufferList, size_t dstOffset, const uint8_t * const * const buffers, size_t srcOffset, size_t byteCount) noexcept
	{
		for(auto bufferIndex = 0; bufferIndex < bufferList->mNumberBuffers; ++bufferIndex) {
			if(dstOffset > bufferList->mBuffers[bufferIndex].mDataByteSize)
				continue;
			memcpy(static_cast<uint8_t *>(bufferList->mBuffers[bufferIndex].mData) + dstOffset, buffers[bufferIndex] + srcOffset, std::min(byteCount, bufferList->mBuffers[bufferIndex].mDataByteSize - dstOffset));
		}
	}

	/*!
	 * Returns the smallest power of two greater than \c x
	 * @param x A value in the range [2..2147483648]
	 * @return The smallest power of two greater than \c x
	 */
	inline constexpr uint32_t NextPowerOfTwo(uint32_t x) noexcept
	{
		assert(x > 1);
		assert(x <= ((std::numeric_limits<uint32_t>::max() / 2) + 1));
		return static_cast<uint32_t>(1 << (32 - __builtin_clz(x - 1)));
	}

}

#pragma mark Creation and Destruction

SFBCARingBuffer::SFBCARingBuffer() noexcept
: mBuffers(nullptr), mCapacityFrames(0), mCapacityFramesMask(0)
{}

SFBCARingBuffer::~SFBCARingBuffer()
{
	Deallocate();
}

#pragma mark Buffer Management

bool SFBCARingBuffer::Allocate(const SFBAudioStreamBasicDescription& format, size_t capacityFrames) noexcept
{
	// Only non-interleaved formats are supported
	if(format.IsInterleaved())
		return false;

	Deallocate();

	// Round up to the next power of two
	capacityFrames = NextPowerOfTwo(static_cast<uint32_t>(capacityFrames));

	mFormat = format;

	mCapacityFrames = capacityFrames;
	mCapacityFramesMask = capacityFrames - 1;

	size_t capacityBytes = capacityFrames * format.mBytesPerFrame;

	// One memory allocation holds everything- first the pointers followed by the deinterleaved channels
	size_t allocationSize = (capacityBytes + sizeof(uint8_t *)) * format.mChannelsPerFrame;
	uint8_t *memoryChunk = static_cast<uint8_t *>(std::malloc(allocationSize));
	if(!memoryChunk)
		return false;

	// Zero the entire allocation
	memset(memoryChunk, 0, allocationSize);

	// Assign the pointers and channel buffers
	mBuffers = reinterpret_cast<uint8_t **>(memoryChunk);
	memoryChunk += format.mChannelsPerFrame * sizeof(uint8_t *);
	for(auto i = 0; i < format.mChannelsPerFrame; ++i) {
		mBuffers[i] = memoryChunk;
		memoryChunk += capacityBytes;
	}

	// Zero the time bounds queue
	for(auto i = 0; i < sTimeBoundsQueueSize; ++i) {
		mTimeBoundsQueue[i].mStartTime = 0;
		mTimeBoundsQueue[i].mEndTime = 0;
		mTimeBoundsQueue[i].mUpdateCounter = 0;
	}

	mTimeBoundsQueueCounter = 0;

	return true;
}

void SFBCARingBuffer::Deallocate() noexcept
{
	if(mBuffers) {
		std::free(mBuffers);
		mBuffers = nullptr;

		mFormat = {};

		mCapacityFrames = 0;
		mCapacityFramesMask = 0;
	}
}

bool SFBCARingBuffer::GetTimeBounds(int64_t& startTime, int64_t& endTime) const noexcept
{
	for(auto i = 0; i < 8; ++i) {
		auto currentCounter = mTimeBoundsQueueCounter.load(std::memory_order_acquire);
		auto currentIndex = currentCounter & sTimeBoundsQueueMask;

		const SFBCARingBuffer::TimeBounds * const bounds = mTimeBoundsQueue + currentIndex;

		startTime = bounds->mStartTime;
		endTime = bounds->mEndTime;

		auto counter = bounds->mUpdateCounter.load(std::memory_order_acquire);

		if(counter == currentCounter)
			return true;
	}

	return false;
}

#pragma mark Reading and Writing Audio

bool SFBCARingBuffer::Read(AudioBufferList * const bufferList, size_t frameCount, int64_t startRead) noexcept
{
	if(frameCount == 0)
		return true;

	startRead = std::max(0LL, startRead);

	int64_t endRead = startRead + frameCount;

	auto startRead0 = startRead;
	auto endRead0 = endRead;

	if(!ClampTimesToBounds(startRead, endRead))
		return false;

	if(startRead == endRead) {
		ZeroABL(bufferList, 0, frameCount * mFormat.mBytesPerFrame);
		return true;
	}

	size_t byteSize = (endRead - startRead) * mFormat.mBytesPerFrame;

	size_t destStartByteOffset = std::max(0LL, (startRead - startRead0) * mFormat.mBytesPerFrame);
	if(destStartByteOffset > 0)
		ZeroABL(bufferList, 0, std::min(frameCount * mFormat.mBytesPerFrame, destStartByteOffset));

	size_t destEndSize = std::max(0LL, endRead0 - endRead);
	if (destEndSize > 0)
		ZeroABL(bufferList, destStartByteOffset + byteSize, destEndSize * mFormat.mBytesPerFrame);

	auto offset0 = FrameByteOffset(startRead);
	auto offset1 = FrameByteOffset(endRead);
	size_t byteCount;

	if (offset0 < offset1) {
		byteCount = offset1 - offset0;
		FetchABL(bufferList, destStartByteOffset, mBuffers, offset0, byteCount);
	}
	else {
		byteCount = static_cast<UInt32>((mCapacityFrames * mFormat.mBytesPerFrame) - offset0);
		FetchABL(bufferList, destStartByteOffset, mBuffers, offset0, byteCount);
		FetchABL(bufferList, destStartByteOffset + byteCount, mBuffers, 0, offset1);
		byteCount += offset1;
	}

	for(auto bufferIndex = 0; bufferIndex < bufferList->mNumberBuffers; ++bufferIndex)
		bufferList->mBuffers[bufferIndex].mDataByteSize = static_cast<UInt32>(byteCount);

	return true;
}

bool SFBCARingBuffer::Write(const AudioBufferList * const bufferList, size_t frameCount, int64_t startWrite) noexcept
{
	if(frameCount == 0)
		return true;

	if(frameCount > mCapacityFrames)
		return false;

	int64_t endWrite = startWrite + frameCount;

	if(startWrite < EndTime()) {
		// Going backwards, throw everything out
		SetTimeBounds(startWrite, startWrite);
	}
	else if(endWrite - StartTime() <= mCapacityFrames) {
		// The buffer has not yet wrapped and will not need to
	}
	else {
		// Advance the start time past the region about to be overwritten
		int64_t newStart = endWrite - mCapacityFrames;	// one buffer of time behind the write position
		int64_t newEnd = std::max(newStart, EndTime());
		SetTimeBounds(newStart, newEnd);
	}

	size_t offset0, offset1;
	auto curEnd = EndTime();

	if(startWrite > curEnd) {
		// Zero the range of samples being skipped
		offset0 = FrameByteOffset(curEnd);
		offset1 = FrameByteOffset(startWrite);
		if(offset0 < offset1)
			ZeroRange(mBuffers, mFormat.ChannelStreamCount(), offset0, offset1 - offset0);
		else {
			ZeroRange(mBuffers, mFormat.ChannelStreamCount(), offset0, (mCapacityFrames * mFormat.mBytesPerFrame) - offset0);
			ZeroRange(mBuffers, mFormat.ChannelStreamCount(), 0, offset1);
		}

		offset0 = offset1;
	}
	else
		offset0 = FrameByteOffset(startWrite);

	offset1 = FrameByteOffset(endWrite);
	if(offset0 < offset1)
		StoreABL(mBuffers, offset0, bufferList, 0, offset1 - offset0);
	else {
		size_t byteCount = (mCapacityFrames * mFormat.mBytesPerFrame) - offset0;
		StoreABL(mBuffers, offset0, bufferList, 0, byteCount);
		StoreABL(mBuffers, 0, bufferList, byteCount, offset1);
	}

	// Update the end time
	SetTimeBounds(StartTime(), endWrite);

	return true;
}

#pragma mark Internals

void SFBCARingBuffer::SetTimeBounds(int64_t startTime, int64_t endTime) noexcept
{
	auto nextCounter = mTimeBoundsQueueCounter.load(std::memory_order_acquire) + 1;
	auto nextIndex = nextCounter & sTimeBoundsQueueMask;

	mTimeBoundsQueue[nextIndex].mStartTime = startTime;
	mTimeBoundsQueue[nextIndex].mEndTime = endTime;
	mTimeBoundsQueue[nextIndex].mUpdateCounter.store(nextCounter, std::memory_order_release);

	mTimeBoundsQueueCounter.fetch_add(1, std::memory_order_release);
}

bool SFBCARingBuffer::ClampTimesToBounds(int64_t& startRead, int64_t& endRead) const noexcept
{
	int64_t startTime, endTime;
	if(!GetTimeBounds(startTime, endTime))
		return false;

	if(startRead > endTime || endRead < startTime) {
		endRead = startRead;
		return true;
	}

	startRead = std::max(startRead, startTime);
	endRead = clamp(endRead, startTime, endTime);

	return true;
}
