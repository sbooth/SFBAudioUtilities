//
// Copyright (c) 2021 - 2024 Stephen F. Booth <me@sbooth.org>
// Part of https://github.com/sbooth/SFBAudioUtilities
// MIT license
//

#pragma once

#import "SFBCAAudioObject.hpp"
#import "SFBCAAudioDevice.hpp"

namespace SFB {

class CAAudioSystemObject : public CAAudioObject
{

public:

	/// Creates a @c CAAudioSystemObject
	inline CAAudioSystemObject() noexcept
	: CAAudioObject{kAudioObjectSystemObject}
	{}

	/// Copy constructor
	CAAudioSystemObject(const CAAudioSystemObject& rhs) = default;

	/// Assignment operator
	CAAudioSystemObject& operator=(const CAAudioSystemObject& rhs) = default;

	/// Destructor
	virtual ~CAAudioSystemObject() = default;

	// Move constructor
	CAAudioSystemObject(CAAudioSystemObject&& rhs) = default;

	// Move assignment operator
	CAAudioSystemObject& operator=(CAAudioSystemObject&& rhs) = default;


	inline std::vector<AudioObjectID> DeviceIDs() const
	{
		return ArrayProperty<AudioObjectID>(CAPropertyAddress(kAudioHardwarePropertyDevices));
	}

	std::vector<CAAudioDevice> Devices() const
	{
		auto vec = DeviceIDs();
		std::vector<CAAudioDevice> result(vec.size());
		std::transform(vec.cbegin(), vec.cend(), result.begin(), [](AudioObjectID objectID) { return CAAudioDevice(objectID); });
		return result;
	}

	inline AudioObjectID DefaultInputDeviceID() const
	{
		return ArithmeticProperty<AudioObjectID>(CAPropertyAddress(kAudioHardwarePropertyDefaultInputDevice));
	}

	inline CAAudioObject DefaultInputDevice() const
	{
		return CAAudioObject(DefaultInputDeviceID());
	}

	inline AudioObjectID DefaultOutputDeviceID() const
	{
		return ArithmeticProperty<AudioObjectID>(CAPropertyAddress(kAudioHardwarePropertyDefaultOutputDevice));
	}

	inline CAAudioObject DefaultOutputDevice() const
	{
		return CAAudioObject(DefaultOutputDeviceID());
	}

	inline AudioObjectID DefaultSystemOutputDeviceID() const
	{
		return ArithmeticProperty<AudioObjectID>(CAPropertyAddress(kAudioHardwarePropertyDefaultSystemOutputDevice));
	}

	inline CAAudioObject DefaultSystemOutputDevice() const
	{
		return CAAudioObject(DefaultSystemOutputDeviceID());
	}

	AudioObjectID AudioDeviceIDForUID(CFStringRef _Nonnull inUID) const
	{
		AudioObjectID deviceID;
		AudioValueTranslation valueTranslation = { &inUID, sizeof(CFStringRef), &deviceID, sizeof(AudioObjectID) };
		CAPropertyAddress objectPropertyAddress(kAudioHardwarePropertyDeviceForUID);
		UInt32 size = sizeof(AudioValueTranslation);
		GetPropertyData(objectPropertyAddress, 0, nullptr, size, &valueTranslation);
		return deviceID;
	}

	inline CAAudioDevice AudioDeviceForUID(CFStringRef _Nonnull inUID) const
	{
		return CAAudioDevice(AudioDeviceIDForUID(inUID));
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

} // namespace SFB
