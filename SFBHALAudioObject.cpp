//
// Copyright (c) 2021 Stephen F. Booth <me@sbooth.org>
// MIT license
//

#import "SFBHALAudioObject.hpp"

#import "SFBCAException.hpp"

bool SFBHALAudioObject::HasProperty(const AudioObjectPropertyAddress& inAddress) const noexcept
{
	return AudioObjectHasProperty(mObjectID, &inAddress);
}

bool SFBHALAudioObject::IsPropertySettable(const AudioObjectPropertyAddress& inAddress) const
{
	Boolean settable = false;
	auto result = AudioObjectIsPropertySettable(mObjectID, &inAddress, &settable);
	SFBThrowIfAudioObjectError(result, "AudioObjectIsPropertySettable");
	return settable != 0;
}

UInt32 SFBHALAudioObject::GetPropertyDataSize(const AudioObjectPropertyAddress& inAddress, UInt32 inQualifierDataSize, const void* inQualifierData) const
{
	UInt32 size = 0;
	auto result = AudioObjectGetPropertyDataSize(mObjectID, &inAddress, inQualifierDataSize, inQualifierData, &size);
	SFBThrowIfAudioObjectError(result, "AudioObjectGetPropertyDataSize");
	return size;
}

void SFBHALAudioObject::GetPropertyData(const AudioObjectPropertyAddress& inAddress, UInt32 inQualifierDataSize, const void *inQualifierData, UInt32& ioDataSize, void *outData) const
{
	auto result = AudioObjectGetPropertyData(mObjectID, &inAddress, inQualifierDataSize, inQualifierData, &ioDataSize, outData);
	SFBThrowIfAudioObjectError(result, "AudioObjectGetPropertyData");
}

void SFBHALAudioObject::SetPropertyData(const AudioObjectPropertyAddress& inAddress, UInt32 inQualifierDataSize, const void *inQualifierData, UInt32 inDataSize, const void *inData)
{
	auto result = AudioObjectSetPropertyData(mObjectID, &inAddress, inQualifierDataSize, inQualifierData, inDataSize, inData);
	SFBThrowIfAudioObjectError(result, "AudioObjectSetPropertyData");
}

void SFBHALAudioObject::AddPropertyListener(const AudioObjectPropertyAddress& inAddress, AudioObjectPropertyListenerProc inListenerProc, void *inClientData)
{
	auto result = AudioObjectAddPropertyListener(mObjectID, &inAddress, inListenerProc, inClientData);
	SFBThrowIfAudioObjectError(result, "AudioObjectAddPropertyListener");
}

void SFBHALAudioObject::RemovePropertyListener(const AudioObjectPropertyAddress& inAddress, AudioObjectPropertyListenerProc inListenerProc, void *inClientData)
{
	auto result = AudioObjectRemovePropertyListener(mObjectID, &inAddress, inListenerProc, inClientData);
	SFBThrowIfAudioObjectError(result, "AudioObjectRemovePropertyListener");
}

void SFBHALAudioObject::AddPropertyListenerBlock(const AudioObjectPropertyAddress& inAddress, dispatch_queue_t inDispatchQueue, AudioObjectPropertyListenerBlock inListenerBlock)
{
	auto result = AudioObjectAddPropertyListenerBlock(mObjectID, &inAddress, inDispatchQueue, inListenerBlock);
	SFBThrowIfAudioObjectError(result, "AudioObjectAddPropertyListenerBlock");
}

void SFBHALAudioObject::RemovePropertyListenerBlock(const AudioObjectPropertyAddress& inAddress, dispatch_queue_t inDispatchQueue, AudioObjectPropertyListenerBlock inListenerBlock)
{
	auto result = AudioObjectRemovePropertyListenerBlock(mObjectID, &inAddress, inDispatchQueue, inListenerBlock);
	SFBThrowIfAudioObjectError(result, "AudioObjectRemovePropertyListenerBlock");
}
