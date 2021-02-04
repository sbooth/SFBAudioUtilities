//
// Copyright (c) 2013 - 2021 Stephen F. Booth <me@sbooth.org>
// MIT license
//

#import <algorithm>
#import <cstdlib>
#import <limits>
#import <new>

#import "SFBAudioBufferList.hpp"

AudioBufferList * SFBAllocateAudioBufferList(const SFBAudioStreamBasicDescription& format, UInt32 frameCapacity) noexcept
{
	if(format.mBytesPerFrame == 0 || frameCapacity > (std::numeric_limits<UInt32>::max() / format.mBytesPerFrame))
		return nullptr;

	auto bufferDataSize = format.FrameCountToByteSize(frameCapacity);
	auto bufferCount = format.ChannelStreamCount();
	auto bufferListSize = offsetof(AudioBufferList, mBuffers) + (sizeof(AudioBuffer) * bufferCount);
	auto allocationSize = bufferListSize + (bufferDataSize * bufferCount);

	auto abl = static_cast<AudioBufferList *>(std::malloc(allocationSize));
	if(!abl)
		return nullptr;

	std::memset(abl, 0, allocationSize);

	abl->mNumberBuffers = bufferCount;

	for(auto i = 0; i < bufferCount; ++i) {
		abl->mBuffers[i].mNumberChannels = format.InterleavedChannelCount();
		abl->mBuffers[i].mData = reinterpret_cast<uint8_t *>(abl) + bufferListSize + (bufferDataSize * i);
		abl->mBuffers[i].mDataByteSize = bufferDataSize;
	}

	return abl;
}

SFBAudioBufferList::SFBAudioBufferList() noexcept
: mBufferList(nullptr), mFrameCapacity(0), mFrameLength(0)
{}

SFBAudioBufferList::~SFBAudioBufferList()
{
	std::free(mBufferList);
}

SFBAudioBufferList::SFBAudioBufferList(SFBAudioBufferList&& rhs)
: mBufferList(rhs.mBufferList), mFormat(rhs.mFormat), mFrameCapacity(rhs.mFrameCapacity), mFrameLength(rhs.mFrameLength)
{
	rhs.mBufferList = nullptr;
	rhs.mFormat.Reset();
	rhs.mFrameCapacity = 0;
	rhs.mFrameLength = 0;
}

SFBAudioBufferList& SFBAudioBufferList::operator=(SFBAudioBufferList&& rhs)
{
	if(this != &rhs) {
		Deallocate();

		mBufferList = rhs.mBufferList;
		mFormat = rhs.mFormat;
		mFrameCapacity = rhs.mFrameCapacity;
		mFrameLength = rhs.mFrameLength;

		rhs.mBufferList = nullptr;
		rhs.mFormat.Reset();
		rhs.mFrameCapacity = 0;
		rhs.mFrameLength = 0;
	}

	return *this;
}

SFBAudioBufferList::SFBAudioBufferList(const SFBAudioStreamBasicDescription& format, UInt32 frameCapacity)
: SFBAudioBufferList()
{
	if(format.mBytesPerFrame == 0)
		throw std::invalid_argument("format.mBytesPerFrame == 0");
	if(!Allocate(format, frameCapacity))
		throw std::bad_alloc();
}

#pragma mark Buffer Management

bool SFBAudioBufferList::Allocate(const SFBAudioStreamBasicDescription& format, UInt32 frameCapacity) noexcept
{
	if(mBufferList)
		Deallocate();

	mBufferList = SFBAllocateAudioBufferList(format, frameCapacity);
	if(!mBufferList)
		return false;

	mFormat = format;
	mFrameCapacity = frameCapacity;
	mFrameLength = 0;

	return true;
}

bool SFBAudioBufferList::Deallocate() noexcept
{
	if(!mBufferList)
		return false;

	std::free(mBufferList);
	mFormat.Reset();
	mFrameCapacity = 0;
	mFrameLength = 0;

	return true;
}

bool SFBAudioBufferList::SetFrameLength(UInt32 frameLength) noexcept
{
	if(!mBufferList || frameLength > mFrameCapacity)
		return false;

	mFrameLength = frameLength;

	for(auto i = 0; i < mBufferList->mNumberBuffers; ++i)
		mBufferList->mBuffers[i].mDataByteSize = mFrameLength * mFormat.mBytesPerFrame;

	return true;
}

bool SFBAudioBufferList::InferFrameLengthFromABL()
{
	if(!mBufferList)
		return false;

	auto buffer0ByteSize = mBufferList->mBuffers[0].mDataByteSize;
	for(auto i = 0; i < mBufferList->mNumberBuffers; ++i) {
		if(mBufferList->mBuffers[i].mDataByteSize != buffer0ByteSize)
			throw std::logic_error("inconsistent values for mBufferList->mBuffers[].mBytesPerFrame");
	}

	auto frameLength = buffer0ByteSize / mFormat.mBytesPerFrame;
	if(frameLength > mFrameCapacity)
		throw std::logic_error("mBufferList->mBuffers[0].mBytesPerFrame / mFormat.mBytesPerFrame > mFrameCapacity");

	mFrameLength = frameLength;

	return true;
}

#pragma mark Buffer Utilities

UInt32 SFBAudioBufferList::InsertFromBuffer(const SFBAudioBufferList& buffer, UInt32 readOffset, UInt32 frameLength, UInt32 writeOffset) noexcept
{
	if(mFormat != buffer.mFormat)
//		throw std::invalid_argument("mFormat != buffer.mFormat");
		return 0;

	if(readOffset > buffer.mFrameLength || writeOffset > mFrameLength || frameLength == 0 || buffer.mFrameLength == 0)
		return 0;

	auto framesToInsert = std::min(mFrameCapacity - mFrameLength, std::min(frameLength, buffer.mFrameLength - readOffset));

	auto framesToMove = mFrameLength - writeOffset;
	if(framesToMove) {
		auto moveToOffset = writeOffset + framesToInsert;
		for(auto i = 0; i < mBufferList->mNumberBuffers; ++i) {
			auto dst = static_cast<uint8_t *>(mBufferList->mBuffers[i].mData) + (moveToOffset * mFormat.mBytesPerFrame);
			const auto src = static_cast<const uint8_t *>(mBufferList->mBuffers[i].mData) + (writeOffset * mFormat.mBytesPerFrame);
			std::memmove(dst, src, framesToMove * mFormat.mBytesPerFrame);
		}
	}

	if(framesToInsert) {
		for(auto i = 0; i < buffer.mBufferList->mNumberBuffers; ++i) {
			auto dst = static_cast<uint8_t *>(mBufferList->mBuffers[i].mData) + (writeOffset * mFormat.mBytesPerFrame);
			const auto src = static_cast<const uint8_t *>(buffer.mBufferList->mBuffers[i].mData) + (readOffset * mFormat.mBytesPerFrame);
			std::memcpy(dst, src, framesToInsert * mFormat.mBytesPerFrame);
		}

		SetFrameLength(mFrameLength + framesToInsert);
	}

	return framesToInsert;
}

UInt32 SFBAudioBufferList::TrimAtOffset(UInt32 offset, UInt32 frameLength) noexcept
{
	if(offset > mFrameLength || frameLength == 0)
		return 0;

	auto framesToTrim = std::min(frameLength, mFrameLength - offset);

	auto framesToMove = mFrameLength - (offset + framesToTrim);
	if(framesToMove) {
		auto moveFromOffset = offset + framesToTrim;
		for(auto i = 0; i < mBufferList->mNumberBuffers; ++i) {
			auto dst = static_cast<uint8_t *>(mBufferList->mBuffers[i].mData) + (offset * mFormat.mBytesPerFrame);
			const auto src = static_cast<const uint8_t *>(mBufferList->mBuffers[i].mData) + (moveFromOffset * mFormat.mBytesPerFrame);
			std::memmove(dst, src, framesToMove * mFormat.mBytesPerFrame);
		}
	}

	SetFrameLength(mFrameLength - framesToTrim);

	return framesToTrim;
}

UInt32 SFBAudioBufferList::InsertSilence(UInt32 offset, UInt32 frameLength) noexcept
{
	if(!(mFormat.IsFloat() || mFormat.IsSignedInteger()))
//		throw std::logic_error("Inserting silence for unsigned integer samples not supported");
		return 0;

	if(offset > mFrameLength || frameLength == 0)
		return 0;

	auto framesToZero = std::min(mFrameCapacity - mFrameLength, frameLength);

	auto framesToMove = mFrameLength - offset;
	if(framesToMove) {
		auto moveToOffset = offset + framesToZero;
		for(auto i = 0; i < mBufferList->mNumberBuffers; ++i) {
			auto dst = static_cast<uint8_t *>(mBufferList->mBuffers[i].mData) + (moveToOffset * mFormat.mBytesPerFrame);
			const auto src = static_cast<const uint8_t *>(mBufferList->mBuffers[i].mData) + (offset * mFormat.mBytesPerFrame);
			std::memmove(dst, src, framesToMove * mFormat.mBytesPerFrame);
		}
	}

	if(framesToZero) {
		// For floating-point numbers this code is non-portable: the C standard doesn't require IEEE 754 compliance
		// However, setting all bits to 0 using memset() on macOS results in a floating-point value of 0
		for(auto i = 0; i < mBufferList->mNumberBuffers; ++i) {
			auto dst = static_cast<uint8_t *>(mBufferList->mBuffers[i].mData) + (offset * mFormat.mBytesPerFrame);
			std::memset(dst, 0, framesToZero * mFormat.mBytesPerFrame);
		}

		SetFrameLength(mFrameLength + framesToZero);
	}

	return framesToZero;
}

bool SFBAudioBufferList::AdoptABL(AudioBufferList *bufferList, const AudioStreamBasicDescription& format, UInt32 frameCapacity, UInt32 frameLength) noexcept
{
	if(!bufferList || frameLength > frameCapacity)
		return false;

	Deallocate();

	mBufferList = bufferList;
	mFormat = format;
	mFrameCapacity = frameCapacity;
	SetFrameLength(frameLength);

	return true;
}

AudioBufferList * SFBAudioBufferList::RelinquishABL() noexcept
{
	auto bufferList = mBufferList;

	mBufferList = nullptr;
	mFormat.Reset();
	mFrameCapacity = 0;
	mFrameLength = 0;

	return bufferList;
}
