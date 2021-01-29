/*
 * Copyright (c) 2013 - 2021 Stephen F. Booth <me@sbooth.org>
 * MIT license
 */

#pragma once

#import <AudioToolbox/AudioToolbox.h>

#import "SFBAudioStreamBasicDescription.hpp"

/*!
 * Allocates and returns a new @c AudioBufferList in a single allocation
 * @note The allocation is performed using @c std::malloc and should be deallocated using @c std::free
 * @param format The format of the audio the @c AudioBufferList will hold
 * @param frameCapacity The desired buffer capacity in audio frames
 * @return A newly-allocated @c AudioBufferList or @c nullptr
 */
AudioBufferList * SFBAllocateAudioBufferList(const SFBAudioStreamBasicDescription& format, UInt32 frameCapacity) noexcept;

/*! A class wrapping a Core Audio @c AudioBufferList */
class SFBAudioBufferList
{
	
public:
	
#pragma mark Creation and Destruction

	/*! Create a new, empty @c SFBAudioBufferList */
	SFBAudioBufferList() noexcept;

	/*! Destroy the @c SFBAudioBufferList and release all associated resources. */
	~SFBAudioBufferList();

	/*!
	 * Create a new @c SFBAudioBufferList
	 * @param format The format of the audio the @c SFBAudioBufferList will hold
	 * @param frameCapacity The desired buffer capacity in audio frames
	 * @throws @c std::bad_alloc
	 */
	SFBAudioBufferList(const SFBAudioStreamBasicDescription& format, UInt32 frameCapacity);

	/*! Creates a new @c SFBAudioBufferList */
	SFBAudioBufferList(SFBAudioBufferList&& rhs);

	/*! Replaces the buffer */
	SFBAudioBufferList& operator=(SFBAudioBufferList&& rhs);

	/*! @internal This class is non-copyable */
	SFBAudioBufferList(const SFBAudioBufferList& rhs) = delete;

	/*! @internal This class is non-assignable */
	SFBAudioBufferList& operator=(const SFBAudioBufferList& rhs) = delete;

#pragma mark Buffer management

	/*!
	 * Create a new @c SFBAudioBufferList
	 * @param format The format of the audio the @c SFBAudioBufferList will hold
	 * @param frameCapacity The desired buffer capacity in audio frames
	 * @return @c true on sucess, @c false otherwise
	 */
	bool Allocate(const SFBAudioStreamBasicDescription& format, UInt32 frameCapacity) noexcept;

	/*! Deallocate the memory associated with this @c SFBAudioBufferList */
	bool Deallocate() noexcept;


	/*!
	 * Reset the @c SFBAudioBufferList to the default state in preparation for reading
	 * This is equivalent to @c SetFrameLength(FrameCapacity())
	 */
	inline bool Reset() noexcept
	{
		return SetFrameLength(mFrameCapacity);
	}

	/*!
	 * Clears the @c SFBAudioBufferList
	 * This is equivalent to @c SetFrameLength(0)
	 * @return @c true on sucess, @c false otherwise
	 */
	inline bool Clear() noexcept
	{
		return SetFrameLength(0);
	}


	/*! Returns the length in audio frames of the data in this @c SFBAudioBufferList */
	UInt32 FrameLength() const noexcept;

	/*!
	 * Set the length in audio frames of the data in this @c SFBAudioBufferList
	 * @param frameLength The number of valid audio frames
	 * @return @c true on sucess, @c false otherwise
	 */
	bool SetFrameLength(UInt32 frameLength) noexcept;


	/*! Get the capacity of this @c SFBAudioBufferList in audio frames */
	inline UInt32 FrameCapacity() const noexcept
	{
		return mFrameCapacity;
	}

	/*! Get the format of this @c SFBAudioBufferList */
	inline const SFBAudioStreamBasicDescription& Format() const noexcept
	{
		return mFormat;
	}

#pragma mark AudioBufferList access

	/*!
	 * Relinquishes ownership of the object's internal @c AudioBufferList and returns it
	 * @note The caller assumes responsiblity for deallocating the returned @c AudioBufferList using @c std::free
	 */
	AudioBufferList * RelinquishABL() noexcept;

	/*! Retrieve a pointer to this object's internal @c AudioBufferList */
	inline AudioBufferList * const ABL() noexcept
	{
		return mBufferList;
	}

	/*! Retrieve a const pointer to this object's internal @c AudioBufferList */
	inline const AudioBufferList * const ABL() const noexcept
	{
		return mBufferList;
	}


	/*! Query whether this @c SFBAudioBufferList is empty */
	inline explicit operator bool() const noexcept
	{
		return mBufferList != nullptr;
	}

	/*! Query whether this @c SFBAudioBufferList is not empty */
	inline bool operator!() const noexcept
	{
		return !mBufferList;
	}


	/*! Retrieve a pointer to this object's internal @c AudioBufferList */
	inline AudioBufferList * const operator->() noexcept
	{
		return mBufferList;
	}

	/*! Retrieve a const pointer to this object's internal @c AudioBufferList */
	inline const AudioBufferList * const operator->() const noexcept
	{
		return mBufferList;
	}

	/*! Retrieve a pointer to this object's internal @c AudioBufferList */
	inline operator AudioBufferList * const () noexcept
	{
		return mBufferList;
	}

	/*! Retrieve a const pointer to this object's internal @c AudioBufferList */
	inline operator const AudioBufferList * const () const noexcept
	{
		return mBufferList;
	}

private:

	AudioBufferList *mBufferList;
	SFBAudioStreamBasicDescription mFormat;
	UInt32 mFrameCapacity;

};
