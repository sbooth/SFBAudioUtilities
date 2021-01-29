/*
 * Copyright (c) 2013 - 2021 Stephen F. Booth <me@sbooth.org>
 * MIT license
 */

#pragma once

#import <vector>

#import <AudioToolbox/AudioToolbox.h>

#import "SFBCFWrapper.hpp"

/*!
 * Returns the size of an @c AudioChannelLayout struct
 * @param channelLayout A pointer to an @c AudioChannelLayout struct
 * @return The size of @c channelLayout in bytes
 */
size_t SFBAudioChannelLayoutSize(const AudioChannelLayout *channelLayout) noexcept;

/*! A class wrapping a Core %Audio @c AudioChannelLayout */
class SFBAudioChannelLayout
{

public:

	/*! Mono layout */
	static const SFBAudioChannelLayout Mono;

	/*! Stereo layout */
	static const SFBAudioChannelLayout Stereo;

	// ========================================
	/*! @name Factory Methods */
	//@{

	/*!
	 * Creates a @c SFBAudioChannelLayout
	 * @param channelBitmap The channel bitmap for the channel layout
	 * @return A @c SFBAudioChannelLayout
	 */
	static SFBAudioChannelLayout ChannelLayoutWithBitmap(UInt32 channelBitmap);

	//@}


	// ========================================
	/*! @name Creation and Destruction */
	//@{

	/*! Creates an empty @c SFBAudioChannelLayout */
	SFBAudioChannelLayout() noexcept;

	/*!
	 * Creates a @c SFBAudioChannelLayout
	 * @param layoutTag The layout tag for the channel layout
	 */
	SFBAudioChannelLayout(AudioChannelLayoutTag layoutTag);

	/*!
	 * Creates a @c SFBAudioChannelLayout
	 * @param channelLabels A @c std::vector of the desired channel labels
	 */
	SFBAudioChannelLayout(std::vector<AudioChannelLabel> channelLabels);

	/*! Destroy the @c SFBAudioChannelLayout and release all associated resources. */
	~SFBAudioChannelLayout();

	/*! Creates a new @c SFBAudioChannelLayout by performing a deep copy of @c channelLayout */
	SFBAudioChannelLayout(const AudioChannelLayout *channelLayout);

	/*! @internal Move constructor */
	SFBAudioChannelLayout(SFBAudioChannelLayout&& rhs) noexcept;

	/*! @internal Move assignment operator */
	SFBAudioChannelLayout& operator=(SFBAudioChannelLayout&& rhs) noexcept;

	/*! @internal Copy constructor */
	SFBAudioChannelLayout(const SFBAudioChannelLayout& rhs);

	/*! @internal Assignment operator */
	SFBAudioChannelLayout& operator=(const SFBAudioChannelLayout& rhs);

	/*! Makes a deep copy of rhs */
	SFBAudioChannelLayout& operator=(const AudioChannelLayout *rhs);

	//@}


	// ========================================
	/*! @name Comparison */
	//@{

	/*! Compare two @c ChannelLayout objects for equality*/
	bool operator==(const SFBAudioChannelLayout& rhs) const noexcept;

	/*! Compare two @c ChannelLayout objects for inequality*/
	inline bool operator!=(const SFBAudioChannelLayout& rhs) const noexcept
	{
		return !operator==(rhs);
	}

	//@}


	// ========================================
	/*! @name Functionality */
	//@{

	/*! Get the number of channels contained in this channel layout */
	size_t ChannelCount() const noexcept;

	/*!
	 * Creates a channel map for remapping audio from this channel layout
	 * @param outputLayout The output channel layout
	 * @param channelMap A @c std::vector to receive the channel map on success
	 * @return @c true on success, @c false otherwise
	 */
	bool MapToLayout(const SFBAudioChannelLayout& outputLayout, std::vector<SInt32>& channelMap) const;

	//@}


	// ========================================
	/*! @name AudioChannelLayout access */
	//@{

	/*! Returns the size in bytes of this object's internal @c AudioChannelLayout */
	inline const size_t Size() const noexcept
	{
		return SFBAudioChannelLayoutSize(mChannelLayout);
	}

	/*!
	 * Relinquishes ownership of the object's internal @c AudioChannelLayout and returns it
	 * @note The caller assumes responsiblity for deallocating the returned @c AudioChannelLayout using @c std::free
	 */
	AudioChannelLayout * RelinquishACL() noexcept;

	/*! Retrieves a const pointer to this object's internal @c AudioChannelLayout */
	inline const AudioChannelLayout * const ACL() const noexcept
	{
		return mChannelLayout;
	}


	/*! Returns @c true if this @c SFBAudioChannelLayout is empty */
	inline explicit operator bool() const noexcept
	{
		return mChannelLayout != nullptr;
	}

	/*! Returns @c true if this @c SFBAudioChannelLayout is not empty */
	inline bool operator!() const noexcept
	{
		return mChannelLayout == nullptr;
	}


	/*! Retrieve a const pointer to this object's internal @c AudioChannelLayout */
	inline const AudioChannelLayout * const  operator->() const noexcept
	{
		return mChannelLayout;
	}

	/*! Retrieve a const pointer to this object's internal @c AudioChannelLayout */
	inline operator const AudioChannelLayout * const () const noexcept
	{
		return mChannelLayout;
	}

	//@}


	/*! Returns a string representation of this channel layout suitable for logging */
	SFBCFString Description(const char * const prefix = nullptr) const noexcept;

private:
	
	AudioChannelLayout *mChannelLayout;

};
