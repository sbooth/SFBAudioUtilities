//
// Copyright (c) 2013 - 2021 Stephen F. Booth <me@sbooth.org>
// MIT license
//

#pragma once

#import <vector>

#import <CoreAudioTypes/CoreAudioTypes.h>

#import "SFBCFWrapper.hpp"

/// Returns the size of an @c AudioChannelLayout struct
/// @param channelLayout A pointer to an @c AudioChannelLayout struct
/// @return The size of @c channelLayout in bytes
size_t SFBAudioChannelLayoutSize(const AudioChannelLayout * _Nullable channelLayout) noexcept;

/// A class wrapping a Core Audio @c AudioChannelLayout
class SFBAudioChannelLayout
{

public:

	/// Mono layout
	static const SFBAudioChannelLayout Mono;

	/// Stereo layout
	static const SFBAudioChannelLayout Stereo;

#pragma mark Factory Methods

	/// Creates a @c SFBAudioChannelLayout
	/// @param channelBitmap The channel bitmap for the channel layout
	/// @return A @c SFBAudioChannelLayout
	static SFBAudioChannelLayout ChannelLayoutWithBitmap(UInt32 channelBitmap);

#pragma mark Creation and Destruction

	/// Creates an empty @c SFBAudioChannelLayout
	SFBAudioChannelLayout() noexcept;

	/// Creates a @c SFBAudioChannelLayout
	/// @param layoutTag The layout tag for the channel layout
	SFBAudioChannelLayout(AudioChannelLayoutTag layoutTag);

	/// Creates a @c SFBAudioChannelLayout
	/// @param channelLabels A @c std::vector of the desired channel labels
	SFBAudioChannelLayout(std::vector<AudioChannelLabel> channelLabels);

	/// Destroys the @c SFBAudioChannelLayout and release all associated resources.
	~SFBAudioChannelLayout();

	/// Creates a new @c SFBAudioChannelLayout by performing a deep copy of @c channelLayout
	SFBAudioChannelLayout(const AudioChannelLayout * _Nullable channelLayout);

	/// Move constructor
	SFBAudioChannelLayout(SFBAudioChannelLayout&& rhs) noexcept;

	/// Move assignment operator
	SFBAudioChannelLayout& operator=(SFBAudioChannelLayout&& rhs) noexcept;

	/// Copy constructor
	SFBAudioChannelLayout(const SFBAudioChannelLayout& rhs);

	/// Assignment operator
	SFBAudioChannelLayout& operator=(const SFBAudioChannelLayout& rhs);

	/// Performs a deep copy of @c rhs
	SFBAudioChannelLayout& operator=(const AudioChannelLayout * _Nullable rhs);

#pragma mark Comparison

	/// Returns @c true if @c rhs is equal to @c this
	bool operator==(const SFBAudioChannelLayout& rhs) const noexcept;

	/// Returns @c true if @c rhs is not equal to @c this
	inline bool operator!=(const SFBAudioChannelLayout& rhs) const noexcept
	{
		return !operator==(rhs);
	}

#pragma mark Functionality

	/// Returns the number of channels contained in this channel layout
	size_t ChannelCount() const noexcept;

	/// Creates a channel map for remapping audio from this channel layout
	/// @param outputLayout The output channel layout
	/// @param channelMap A @c std::vector to receive the channel map on success
	/// @return @c true on success, @c false otherwise
	bool MapToLayout(const SFBAudioChannelLayout& outputLayout, std::vector<SInt32>& channelMap) const;

#pragma mark AudioChannelLayout access

	/// Returns the size in bytes of this object's internal @c AudioChannelLayout
	inline const size_t Size() const noexcept
	{
		return SFBAudioChannelLayoutSize(mChannelLayout);
	}

	/// Relinquishes ownership of the object's internal @c AudioChannelLayout and returns it
	/// @note The caller assumes responsiblity for deallocating the returned @c AudioChannelLayout using @c std::free
	AudioChannelLayout * _Nullable RelinquishACL() noexcept;

	/// Retrieves a const pointer to this object's internal @c AudioChannelLayout
	inline const AudioChannelLayout * const _Nullable ACL() const noexcept
	{
		return mChannelLayout;
	}


	/// Returns @c true if this object's internal @c AudioChannelLayout is not @c nullptr
	inline explicit operator bool() const noexcept
	{
		return mChannelLayout != nullptr;
	}

	/// Returns @c true if this object's internal @c AudioChannelLayout is @c nullptr
	inline bool operator!() const noexcept
	{
		return !mChannelLayout;
	}


	/// Retrieve a const pointer to this object's internal @c AudioChannelLayout
	inline const AudioChannelLayout * const _Nullable operator->() const noexcept
	{
		return mChannelLayout;
	}

	/// Retrieve a const pointer to this object's internal @c AudioChannelLayout
	inline operator const AudioChannelLayout * const _Nullable () const noexcept
	{
		return mChannelLayout;
	}


	/// Returns a string representation of this channel layout suitable for logging
	SFBCFString Description(const char * const _Nullable prefix = nullptr) const noexcept;

private:

	/// The underlying @c AudioChannelLayout struct
	AudioChannelLayout * _Nullable mChannelLayout;

};
