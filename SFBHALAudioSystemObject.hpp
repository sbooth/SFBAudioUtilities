//
// Copyright (c) 2021 Stephen F. Booth <me@sbooth.org>
// MIT license
//

#pragma once

#import "SFBHALAudioObject.hpp"
#import "SFBHALAudioDevice.hpp"

class SFBHALAudioSystemObject : public SFBHALAudioObject
{

public:

	/// Creates an @c SFBHALAudioSystemObject
	inline SFBHALAudioSystemObject() noexcept
	: SFBHALAudioObject(kAudioObjectSystemObject)
	{}

	/// Copy constructor
	SFBHALAudioSystemObject(const SFBHALAudioSystemObject& rhs) = default;

	/// Assignment operator
	SFBHALAudioSystemObject& operator=(const SFBHALAudioSystemObject& rhs) = default;

	/// Destructor
	virtual ~SFBHALAudioSystemObject() = default;

	// Move constructor
	SFBHALAudioSystemObject(SFBHALAudioSystemObject&& rhs) = default;

	// Move assignment operator
	SFBHALAudioSystemObject& operator=(SFBHALAudioSystemObject&& rhs) = default;

	
	inline std::vector<AudioObjectID> DeviceIDs() const
	{
		return ArrayProperty<AudioObjectID>(SFBAudioObjectPropertyAddress(kAudioHardwarePropertyDevices));
	}

	std::vector<SFBHALAudioDevice> Devices() const
	{
		auto vec = DeviceIDs();
		std::vector<SFBHALAudioDevice> result(vec.size());
		std::transform(vec.cbegin(), vec.cend(), result.begin(), [](AudioObjectID objectID) { return SFBHALAudioDevice(objectID); });
		return result;
	}

	inline AudioObjectID DefaultInputDeviceID() const
	{
		return IntegralProperty<AudioObjectID>(SFBAudioObjectPropertyAddress(kAudioHardwarePropertyDefaultInputDevice));
	}

	inline SFBHALAudioObject DefaultInputDevice() const
	{
		return SFBHALAudioObject(DefaultInputDeviceID());
	}

	inline AudioObjectID DefaultOutputDeviceID() const
	{
		return IntegralProperty<AudioObjectID>(SFBAudioObjectPropertyAddress(kAudioHardwarePropertyDefaultOutputDevice));
	}

	inline SFBHALAudioObject DefaultOutputDevice() const
	{
		return SFBHALAudioObject(DefaultOutputDeviceID());
	}

	inline AudioObjectID DefaultSystemOutputDeviceID() const
	{
		return IntegralProperty<AudioObjectID>(SFBAudioObjectPropertyAddress(kAudioHardwarePropertyDefaultSystemOutputDevice));
	}

	inline SFBHALAudioObject DefaultSystemOutputDevice() const
	{
		return SFBHALAudioObject(DefaultSystemOutputDeviceID());
	}

};
