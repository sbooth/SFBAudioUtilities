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
		return ArithmeticProperty<AudioObjectID>(SFBAudioObjectPropertyAddress(kAudioHardwarePropertyDefaultInputDevice));
	}

	inline SFBHALAudioObject DefaultInputDevice() const
	{
		return SFBHALAudioObject(DefaultInputDeviceID());
	}

	inline AudioObjectID DefaultOutputDeviceID() const
	{
		return ArithmeticProperty<AudioObjectID>(SFBAudioObjectPropertyAddress(kAudioHardwarePropertyDefaultOutputDevice));
	}

	inline SFBHALAudioObject DefaultOutputDevice() const
	{
		return SFBHALAudioObject(DefaultOutputDeviceID());
	}

	inline AudioObjectID DefaultSystemOutputDeviceID() const
	{
		return ArithmeticProperty<AudioObjectID>(SFBAudioObjectPropertyAddress(kAudioHardwarePropertyDefaultSystemOutputDevice));
	}

	inline SFBHALAudioObject DefaultSystemOutputDevice() const
	{
		return SFBHALAudioObject(DefaultSystemOutputDeviceID());
	}

	AudioObjectID AudioDeviceIDForUID(CFStringRef _Nonnull inUID) const
	{
		AudioObjectID deviceID;
		AudioValueTranslation valueTranslation = { &inUID, sizeof(CFStringRef), &deviceID, sizeof(AudioObjectID) };
		SFBAudioObjectPropertyAddress objectPropertyAddress(kAudioHardwarePropertyDeviceForUID);
		UInt32 size = sizeof(AudioValueTranslation);
		GetPropertyData(objectPropertyAddress, 0, nullptr, size, &valueTranslation);
		return deviceID;
	}

	inline SFBHALAudioDevice AudioDeviceForUID(CFStringRef _Nonnull inUID) const
	{
		return SFBHALAudioDevice(AudioDeviceIDForUID(inUID));
	}

	//	kAudioHardwarePropertyMixStereoToMono                       = 'stmo',
	//	kAudioHardwarePropertyPlugInList                            = 'plg#',
	//	kAudioHardwarePropertyTranslateBundleIDToPlugIn             = 'bidp',
	//	kAudioHardwarePropertyTransportManagerList                  = 'tmg#',
	//	kAudioHardwarePropertyTranslateBundleIDToTransportManager   = 'tmbi',
	//	kAudioHardwarePropertyBoxList                               = 'box#',
	//	kAudioHardwarePropertyTranslateUIDToBox                     = 'uidb',
	//	kAudioHardwarePropertyClockDeviceList                       = 'clk#',
	//	kAudioHardwarePropertyTranslateUIDToClockDevice             = 'uidc',
	//	kAudioHardwarePropertyProcessIsMaster                       = 'mast',
	//	kAudioHardwarePropertyIsInitingOrExiting                    = 'inot',
	//	kAudioHardwarePropertyUserIDChanged                         = 'euid',
	//	kAudioHardwarePropertyProcessIsAudible                      = 'pmut',
	//	kAudioHardwarePropertySleepingIsAllowed                     = 'slep',
	//	kAudioHardwarePropertyUnloadingIsAllowed                    = 'unld',
	//	kAudioHardwarePropertyHogModeIsAllowed                      = 'hogr',
	//	kAudioHardwarePropertyUserSessionIsActiveOrHeadless         = 'user',
	//	kAudioHardwarePropertyServiceRestarted                      = 'srst',
	//	kAudioHardwarePropertyPowerHint                             = 'powh'

};
