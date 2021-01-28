/*
 * Copyright (c) 2013 - 2021 Stephen F. Booth <me@sbooth.org>
 * MIT license
 */

#pragma once

#import <vector>

#import <AudioToolbox/AudioToolbox.h>

#import "SFBCFWrapper.hpp"

/*!
 * @brief Returns the size of an \c AudioChannelLayout struct
 * @param channelLayout A pointer to an \c AudioChannelLayout struct
 * @return The size of \c channelLayout in bytes
 */
size_t SFBAudioChannelLayoutSize(const AudioChannelLayout *channelLayout) noexcept;

/*! @brief A class wrapping a Core %Audio \c AudioChannelLayout */
class SFBAudioChannelLayout
{

public:

	/*! @brief Mono layout */
	static const SFBAudioChannelLayout Mono;

	/*! @brief Stereo layout */
	static const SFBAudioChannelLayout Stereo;

	// ========================================
	/*! @name Factory Methods */
	//@{

	/*!
	 * @brief Create a \c SFBAudioChannelLayout
	 * @param layoutTag The layout tag for the channel layout
	 * @return A \c SFBAudioChannelLayout
	 */
	static SFBAudioChannelLayout ChannelLayoutWithTag(AudioChannelLayoutTag layoutTag);

	/*!
	 * @brief Create a \c SFBAudioChannelLayout
	 * @param channelLabels A \c std::vector of the desired channel labels
	 * @return A \c SFBAudioChannelLayout
	 */
	static SFBAudioChannelLayout ChannelLayoutWithChannelLabels(std::vector<AudioChannelLabel> channelLabels);

	/*!
	 * @brief Create a \c SFBAudioChannelLayout
	 * @param channelBitmap The channel bitmap for the channel layout
	 * @return A \c SFBAudioChannelLayout
	 */
	static SFBAudioChannelLayout ChannelLayoutWithBitmap(UInt32 channelBitmap);

	//@}


	// ========================================
	/*! @name Creation and Destruction */
	//@{

	/*! @brief Create a new, empty \c SFBAudioChannelLayout */
	SFBAudioChannelLayout() noexcept;

	/*! @brief Destroy the \c SFBAudioChannelLayout and release all associated resources. */
	~SFBAudioChannelLayout();

	/*! @brief Create a new \c SFBAudioChannelLayout by performing a deep copy of \c channelLayout */
	SFBAudioChannelLayout(const AudioChannelLayout *channelLayout);

	/*! @internal Move constructor */
	SFBAudioChannelLayout(SFBAudioChannelLayout&& rhs) noexcept;

	/*! @internal Move assignment operator */
	SFBAudioChannelLayout& operator=(SFBAudioChannelLayout&& rhs) noexcept;

	/*! @internal Copy constructor */
	SFBAudioChannelLayout(const SFBAudioChannelLayout& rhs);

	/*! @internal Assignment operator */
	SFBAudioChannelLayout& operator=(const SFBAudioChannelLayout& rhs);

	/*! @brief Makes a deep copy of rhs */
	SFBAudioChannelLayout& operator=(const AudioChannelLayout *rhs);

	//@}


	// ========================================
	/*! @name Comparison */
	//@{

	/*! @brief Compare two \c ChannelLayout objects for equality*/
	bool operator==(const SFBAudioChannelLayout& rhs) const noexcept;

	/*! @brief Compare two \c ChannelLayout objects for inequality*/
	inline bool operator!=(const SFBAudioChannelLayout& rhs) const noexcept
	{
		return !operator==(rhs);
	}

	//@}


	// ========================================
	/*! @name Functionality */
	//@{

	/*! @brief Get the number of channels contained in this channel layout */
	size_t ChannelCount() const noexcept;

	/*!
	 * @brief Create a channel map for converting audio from this channel layout
	 * @param outputLayout The output channel layout
	 * @param channelMap A \c std::vector to receive the channel map on success
	 * @return \c true on success, \c false otherwise
	 */
	bool MapToLayout(const SFBAudioChannelLayout& outputLayout, std::vector<SInt32>& channelMap) const;

	//@}


	// ========================================
	/*! @name AudioChannelLayout access */
	//@{

	/*! @brief Retrieve a const pointer to this object's internal \c AudioChannelLayout */
	inline const AudioChannelLayout * ACL() const noexcept
	{
		return mChannelLayout;
	}


	/*! @brief Query whether this \c ChannelLayout is empty */
	inline explicit operator bool() const noexcept
	{
		return mChannelLayout != nullptr;
	}

	/*! @brief Query whether this \c ChannelLayout is not empty */
	inline bool operator!() const noexcept
	{
		return mChannelLayout == nullptr;
	}


	/*! @brief Retrieve a const pointer to this object's internal \c AudioChannelLayout */
	inline const AudioChannelLayout * operator->() const noexcept
	{
		return mChannelLayout;
	}

	/*! @brief Retrieve a const pointer to this object's internal \c AudioChannelLayout */
	inline operator const AudioChannelLayout *() const noexcept
	{
		return mChannelLayout;
	}

	//@}

	/*! @brief Returns a string representation of this channel layout suitable for logging */
	SFBCFString Description(const char * const prefix = nullptr) const noexcept;

private:
	
	AudioChannelLayout *mChannelLayout;

};
