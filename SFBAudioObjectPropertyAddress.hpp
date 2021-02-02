//
// Copyright (c) 2021 Stephen F. Booth <me@sbooth.org>
// MIT license
//

#pragma once

#import <CoreAudio/AudioHardware.h>

/// A class extending the functionality of a Core Audio @c AudioObjectPropertyAddress
class SFBAudioObjectPropertyAddress : public AudioObjectPropertyAddress
{

public:

	/// Creates an empty @c SFBAudioObjectPropertyAddress
	SFBAudioObjectPropertyAddress() noexcept = default;

	/// Copy constructor
	SFBAudioObjectPropertyAddress(const SFBAudioObjectPropertyAddress& rhs) noexcept = default;

	/// Assignment operator
	SFBAudioObjectPropertyAddress& operator=(const SFBAudioObjectPropertyAddress& rhs) noexcept = default;

	/// Destructor
	~SFBAudioObjectPropertyAddress() = default;

	/// Move constructor
	SFBAudioObjectPropertyAddress(SFBAudioObjectPropertyAddress&& rhs) noexcept = default;

	/// Move assignment operator
	SFBAudioObjectPropertyAddress& operator=(SFBAudioObjectPropertyAddress&& rhs) noexcept = default;


	/// Creates an @c SFBAudioObjectPropertyAddress
	/// @param selector The property selector
	/// @param scope The property element
	/// @param element The property scope
	inline SFBAudioObjectPropertyAddress(AudioObjectPropertySelector selector, AudioObjectPropertyScope scope = kAudioObjectPropertyScopeGlobal, AudioObjectPropertyElement element = kAudioObjectPropertyElementMaster) noexcept
	: AudioObjectPropertyAddress{selector, scope, element}
	{}

	// Native overloads

	/// Creates a new @c SFBAudioObjectPropertyAddress for the specified @c AudioObjectPropertyAddress
	inline SFBAudioObjectPropertyAddress(const AudioObjectPropertyAddress& rhs) noexcept
	: AudioObjectPropertyAddress{rhs}
	{}

	/// Assignment operator
	inline SFBAudioObjectPropertyAddress& operator=(const AudioObjectPropertyAddress& rhs) noexcept
	{
		AudioObjectPropertyAddress::operator=(rhs);
		return *this;
	}

	// Comparison not defined because of wildcards

};
