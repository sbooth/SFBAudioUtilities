//
// Copyright (c) 2021 - 2024 Stephen F. Booth <me@sbooth.org>
// Part of https://github.com/sbooth/SFBAudioUtilities
// MIT license
//

#pragma once

#import <cstring>

#import <CoreAudio/AudioHardware.h>

namespace SFB {

/// A class extending the functionality of a Core Audio @c AudioObjectPropertyAddress
class CAPropertyAddress : public AudioObjectPropertyAddress
{

public:

	/// Creates an empty @c CAPropertyAddress
	CAPropertyAddress() noexcept = default;

	/// Copy constructor
	CAPropertyAddress(const CAPropertyAddress& rhs) noexcept = default;

	/// Assignment operator
	CAPropertyAddress& operator=(const CAPropertyAddress& rhs) noexcept = default;

	/// Destructor
	~CAPropertyAddress() = default;

	/// Move constructor
	CAPropertyAddress(CAPropertyAddress&& rhs) noexcept = default;

	/// Move assignment operator
	CAPropertyAddress& operator=(CAPropertyAddress&& rhs) noexcept = default;


	/// Creates a @c CAPropertyAddress
	/// @param selector The property selector
	/// @param scope The property element
	/// @param element The property scope
	inline CAPropertyAddress(AudioObjectPropertySelector selector, AudioObjectPropertyScope scope = kAudioObjectPropertyScopeGlobal, AudioObjectPropertyElement element = kAudioObjectPropertyElementMain) noexcept
	: AudioObjectPropertyAddress{selector, scope, element}
	{}

	// Native overloads

	/// Creates a new @c CAPropertyAddress for the specified @c AudioObjectPropertyAddress
	inline CAPropertyAddress(const AudioObjectPropertyAddress& rhs) noexcept
	: AudioObjectPropertyAddress{rhs}
	{}

	/// Assignment operator
	inline CAPropertyAddress& operator=(const AudioObjectPropertyAddress& rhs) noexcept
	{
		AudioObjectPropertyAddress::operator=(rhs);
		return *this;
	}

#pragma mark Comparison

	/// Returns @c true if @c rhs is equal to @c this
	inline bool operator==(const AudioObjectPropertyAddress& rhs) const noexcept
	{
		return !std::memcmp(this, &rhs, sizeof(AudioObjectPropertyAddress));
	}

	/// Returns @c true if @c rhs is not equal to @c this
	inline bool operator!=(const AudioObjectPropertyAddress& rhs) const noexcept
	{
		return !operator==(rhs);
	}

	/// Returns @c true if @c rhs is congruent to @c this
	/// @note Congruence relations consider wildcards
	inline bool Congruent(const AudioObjectPropertyAddress& rhs) const noexcept
	{
		return (mSelector == rhs.mSelector || mSelector == kAudioObjectPropertySelectorWildcard || rhs.mSelector == kAudioObjectPropertySelectorWildcard)
		&& (mScope == rhs.mScope || mScope == kAudioObjectPropertyScopeWildcard || rhs.mScope == kAudioObjectPropertyScopeWildcard)
		&& (mElement == rhs.mElement || mElement == kAudioObjectPropertyElementWildcard || rhs.mElement == kAudioObjectPropertyElementWildcard);
	}

};

} // namespace SFB
