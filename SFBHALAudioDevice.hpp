//
// Copyright (c) 2021 Stephen F. Booth <me@sbooth.org>
// MIT license
//

#pragma once

#import "SFBHALAudioObject.hpp"
#import "SFBHALAudioStream.hpp"

class SFBHALAudioDevice : public SFBHALAudioObject
{

public:

	/// Creates an unknown @c SFBHALAudioObject
	constexpr SFBHALAudioDevice() noexcept = default;

	/// Copy constructor
	constexpr SFBHALAudioDevice(const SFBHALAudioDevice& rhs) noexcept = default;

	/// Assignment operator
	SFBHALAudioDevice& operator=(const SFBHALAudioDevice& rhs) noexcept = default;

	/// Destructor
	virtual ~SFBHALAudioDevice() = default;

	/// Move constructor
	SFBHALAudioDevice(SFBHALAudioDevice&& rhs) = default;

	/// Move assignment operator
	SFBHALAudioDevice& operator=(SFBHALAudioDevice&& rhs) = default;


	/// Creates a @c SFBHALAudioDevice with the specified objectID
	inline constexpr SFBHALAudioDevice(AudioObjectID objectID) noexcept
	: SFBHALAudioObject(objectID)
	{}


	//	kAudioDevicePropertyConfigurationApplication        = 'capp',

	inline SFBCFString UID() const
	{
		return CFTypeProperty<CFStringRef>(SFBAudioObjectPropertyAddress(kAudioDevicePropertyDeviceUID));
	}

	inline SFBCFString ModelUID() const
	{
		return CFTypeProperty<CFStringRef>(SFBAudioObjectPropertyAddress(kAudioDevicePropertyModelUID));
	}

	//	kAudioDevicePropertyTransportType                   = 'tran',
	//	kAudioDevicePropertyRelatedDevices                  = 'akin',
	//	kAudioDevicePropertyClockDomain                     = 'clkd',
	//	kAudioDevicePropertyDeviceIsAlive                   = 'livn',
	//	kAudioDevicePropertyDeviceIsRunning                 = 'goin',
	//	kAudioDevicePropertyDeviceCanBeDefaultDevice        = 'dflt',
	//	kAudioDevicePropertyDeviceCanBeDefaultSystemDevice  = 'sflt',

	inline UInt32 Latency(SFBHALAudioObjectDirectionalScope scope) const
	{
		return ArithmeticProperty<UInt32>(SFBAudioObjectPropertyAddress(kAudioDevicePropertyLatency, scope == SFBHALAudioObjectDirectionalScope::input ? kAudioObjectPropertyScopeInput : kAudioObjectPropertyScopeOutput));
	}

	inline std::vector<AudioObjectID> StreamIDs(SFBHALAudioObjectDirectionalScope scope) const
	{
		return ArrayProperty<AudioObjectID>(SFBAudioObjectPropertyAddress(kAudioDevicePropertyStreams, scope == SFBHALAudioObjectDirectionalScope::input ? kAudioObjectPropertyScopeInput : kAudioObjectPropertyScopeOutput));
	}

	std::vector<SFBHALAudioStream> Streams(SFBHALAudioObjectDirectionalScope scope) const
	{
		auto vec = StreamIDs(scope);
		std::vector<SFBHALAudioStream> result(vec.size());
		std::transform(vec.cbegin(), vec.cend(), result.begin(), [](AudioObjectID objectID) { return SFBHALAudioStream(objectID); });
		return result;
	}

	//	kAudioObjectPropertyControlList                     = 'ctrl',

	inline UInt32 SafetyOffset(SFBHALAudioObjectDirectionalScope scope) const
	{
		return ArithmeticProperty<UInt32>(SFBAudioObjectPropertyAddress(kAudioDevicePropertySafetyOffset, scope == SFBHALAudioObjectDirectionalScope::input ? kAudioObjectPropertyScopeInput : kAudioObjectPropertyScopeOutput));
	}

	inline Float64 NominalSampleRate() const
	{
		return ArithmeticProperty<Float64>(SFBAudioObjectPropertyAddress(kAudioDevicePropertyNominalSampleRate));
	}

	//	kAudioDevicePropertyAvailableNominalSampleRates     = 'nsr#',
	//	kAudioDevicePropertyIcon                            = 'icon',
	//	kAudioDevicePropertyIsHidden                        = 'hidn',
	//	kAudioDevicePropertyPreferredChannelsForStereo      = 'dch2',
	//	kAudioDevicePropertyPreferredChannelLayout          = 'srnd'

	//	kAudioDevicePropertyPlugIn                          = 'plug',
	//	kAudioDevicePropertyDeviceHasChanged                = 'diff',
	//	kAudioDevicePropertyDeviceIsRunningSomewhere        = 'gone',
	//	kAudioDeviceProcessorOverload                       = 'over',
	//	kAudioDevicePropertyIOStoppedAbnormally             = 'stpd',
	//	kAudioDevicePropertyHogMode                         = 'oink',

	inline UInt32 BufferFrameSize() const
	{
		return ArithmeticProperty<UInt32>(SFBAudioObjectPropertyAddress(kAudioDevicePropertyBufferFrameSize));
	}

	//	kAudioDevicePropertyBufferFrameSizeRange            = 'fsz#',
	//	kAudioDevicePropertyUsesVariableBufferFrameSizes    = 'vfsz',
	//	kAudioDevicePropertyIOCycleUsage                    = 'ncyc',
	//	kAudioDevicePropertyStreamConfiguration             = 'slay',
	//	kAudioDevicePropertyIOProcStreamUsage               = 'suse',
	//	kAudioDevicePropertyActualSampleRate                = 'asrt',
	//	kAudioDevicePropertyClockDevice                     = 'apcd',
	//	kAudioDevicePropertyIOThreadOSWorkgroup				= 'oswg'

};
