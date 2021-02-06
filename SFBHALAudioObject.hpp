//
// Copyright (c) 2021 Stephen F. Booth <me@sbooth.org>
// MIT license
//

#pragma once

#import <vector>

#import <CoreAudio/CoreAudio.h>

#import "SFBAudioObjectPropertyAddress.hpp"
#import "SFBCAException.hpp"
#import "SFBCFWrapper.hpp"

enum class SFBHALAudioObjectDirectionalScope {
	input,
	output,
};

class SFBHALAudioObject
{

public:

#pragma mark Creation and Destruction

	/// Creates an unknown @c SFBHALAudioObject
	inline constexpr SFBHALAudioObject() noexcept
	: mObjectID(kAudioObjectUnknown)
	{}

	/// Copy constructor
	inline constexpr SFBHALAudioObject(const SFBHALAudioObject& rhs) noexcept
	: mObjectID(rhs.mObjectID)
	{}

	/// Assignment operator
	inline SFBHALAudioObject& operator=(const SFBHALAudioObject& rhs) noexcept
	{
		if(this != &rhs)
			mObjectID = rhs.mObjectID;
		return *this;
	}

	/// Destructor
	virtual ~SFBHALAudioObject() = default;

	// Move constructor
	SFBHALAudioObject(SFBHALAudioObject&& rhs) noexcept = default;

	/// Move assignment operator
	SFBHALAudioObject& operator=(SFBHALAudioObject&& rhs) noexcept = default;


	/// Creates a @c SFBHALAudioObject with the specified objectID
	inline constexpr SFBHALAudioObject(AudioObjectID objectID) noexcept
	: mObjectID(objectID)
	{}

#pragma mark Comparison

	/// Returns @c true if this object's @c AudioObjectID is not @c kAudioObjectUnknown
	inline explicit operator bool() const noexcept
	{
		return mObjectID != kAudioObjectUnknown;
	}

	/// Returns @c true if this object's @c AudioObjectID is @c kAudioObjectUnknown
	inline bool operator!() const noexcept
	{
		return !operator bool();
	}

	/// Returns @c true if @c rhs is equal to @c this
	inline bool operator==(AudioObjectID rhs) const noexcept
	{
		return mObjectID == rhs;
	}

	/// Returns @c true if @c rhs is not equal to @c this
	inline bool operator!=(AudioObjectID rhs) const noexcept
	{
		return !operator==(rhs);
	}
	
	inline operator AudioObjectID() const noexcept
	{
		return mObjectID;
	}

	inline AudioObjectID ObjectID() const noexcept
	{
		return mObjectID;
	}

#pragma mark Property Operations

	inline bool HasProperty(const AudioObjectPropertyAddress& inAddress) const noexcept
	{
		return AudioObjectHasProperty(mObjectID, &inAddress);
	}

	bool IsPropertySettable(const AudioObjectPropertyAddress& inAddress) const
	{
		Boolean settable = false;
		auto result = AudioObjectIsPropertySettable(mObjectID, &inAddress, &settable);
		SFBThrowIfAudioObjectError(result, "AudioObjectIsPropertySettable");
		return settable != 0;
	}

	UInt32 GetPropertyDataSize(const AudioObjectPropertyAddress& inAddress, UInt32 inQualifierDataSize = 0, const void * _Nullable inQualifierData = nullptr) const
	{
		UInt32 size = 0;
		auto result = AudioObjectGetPropertyDataSize(mObjectID, &inAddress, inQualifierDataSize, inQualifierData, &size);
		SFBThrowIfAudioObjectError(result, "AudioObjectGetPropertyDataSize");
		return size;
	}


	inline void GetPropertyData(const AudioObjectPropertyAddress& inAddress, UInt32 inQualifierDataSize, const void * _Nullable inQualifierData, UInt32& ioDataSize, void * _Nonnull outData) const
	{
		auto result = AudioObjectGetPropertyData(mObjectID, &inAddress, inQualifierDataSize, inQualifierData, &ioDataSize, outData);
		SFBThrowIfAudioObjectError(result, "AudioObjectGetPropertyData");
	}

	inline void SetPropertyData(const AudioObjectPropertyAddress& inAddress, UInt32 inQualifierDataSize, const void * _Nullable inQualifierData, UInt32 inDataSize, const void * _Nonnull inData)
	{
		auto result = AudioObjectSetPropertyData(mObjectID, &inAddress, inQualifierDataSize, inQualifierData, inDataSize, inData);
		SFBThrowIfAudioObjectError(result, "AudioObjectSetPropertyData");
	}


	template <typename T, typename std::enable_if<std::is_arithmetic<T>::value, bool>::type = true>
	T ArithmeticProperty(const AudioObjectPropertyAddress& inAddress, UInt32 inQualifierDataSize = 0, const void * _Nullable inQualifierData = nullptr) const
	{
		T value;
		UInt32 size = sizeof(T);
		GetPropertyData(inAddress, inQualifierDataSize, inQualifierData, size, &value);
		return value;
	}

	template <typename T, typename std::enable_if<std::is_trivial<T>::value, bool>::type = true>
	T StructProperty(const AudioObjectPropertyAddress& inAddress, UInt32 inQualifierDataSize = 0, const void * _Nullable inQualifierData = nullptr) const
	{
		T value;
		UInt32 size = sizeof(T);
		GetPropertyData(inAddress, inQualifierDataSize, inQualifierData, size, &value);
		return value;
	}

	template <typename T, typename std::enable_if<std::is_trivial<T>::value, bool>::type = true>
	std::vector<T> ArrayProperty(const AudioObjectPropertyAddress& inAddress, UInt32 inQualifierDataSize = 0, const void * _Nullable inQualifierData = nullptr) const
	{
		auto size = GetPropertyDataSize(inAddress, inQualifierDataSize, inQualifierData);
		auto count = size / sizeof(T);
		auto vec = std::vector<T>(count);
		GetPropertyData(inAddress, inQualifierDataSize, inQualifierData, size, &vec[0]);
		return vec;
	}

	template <typename T, typename std::enable_if</*std::is_class<T>::value &&*/ std::is_pointer<T>::value, bool>::type = true>
	SFBCFWrapper<T> CFTypeProperty(const AudioObjectPropertyAddress& inAddress, UInt32 inQualifierDataSize = 0, const void * _Nullable inQualifierData = nullptr) const
	{
		T value;
		UInt32 size = sizeof(T);
		GetPropertyData(inAddress, inQualifierDataSize, inQualifierData, size, &value);
		return SFBCFWrapper<T>(value);
	}

	inline void AddPropertyListener(const AudioObjectPropertyAddress& inAddress, AudioObjectPropertyListenerProc _Nonnull inListenerProc, void * _Nullable inClientData)
	{
		auto result = AudioObjectAddPropertyListener(mObjectID, &inAddress, inListenerProc, inClientData);
		SFBThrowIfAudioObjectError(result, "AudioObjectAddPropertyListener");
	}

	inline void RemovePropertyListener(const AudioObjectPropertyAddress& inAddress, AudioObjectPropertyListenerProc _Nonnull inListenerProc, void * _Nullable inClientData)
	{
		auto result = AudioObjectRemovePropertyListener(mObjectID, &inAddress, inListenerProc, inClientData);
		SFBThrowIfAudioObjectError(result, "AudioObjectRemovePropertyListener");
	}


	inline void AddPropertyListenerBlock(const AudioObjectPropertyAddress& inAddress, dispatch_queue_t _Nonnull inDispatchQueue, AudioObjectPropertyListenerBlock _Nonnull inListenerBlock)
	{
		auto result = AudioObjectAddPropertyListenerBlock(mObjectID, &inAddress, inDispatchQueue, inListenerBlock);
		SFBThrowIfAudioObjectError(result, "AudioObjectAddPropertyListenerBlock");
	}

	inline void RemovePropertyListenerBlock(const AudioObjectPropertyAddress& inAddress, dispatch_queue_t _Nonnull inDispatchQueue, AudioObjectPropertyListenerBlock _Nonnull inListenerBlock)
	{
		auto result = AudioObjectRemovePropertyListenerBlock(mObjectID, &inAddress, inDispatchQueue, inListenerBlock);
		SFBThrowIfAudioObjectError(result, "AudioObjectRemovePropertyListenerBlock");
	}

#pragma mark AudioObject Properties

	inline AudioClassID BaseClass() const
	{
		return ArithmeticProperty<AudioClassID>(SFBAudioObjectPropertyAddress(kAudioObjectPropertyBaseClass));
	}

	inline AudioClassID Class() const
	{
		return ArithmeticProperty<AudioClassID>(SFBAudioObjectPropertyAddress(kAudioObjectPropertyClass));
	}

	inline AudioObjectID OwnerID() const
	{
		return ArithmeticProperty<AudioObjectID>(SFBAudioObjectPropertyAddress(kAudioObjectPropertyOwner));
	}

	inline SFBHALAudioObject Owner() const
	{
		return SFBHALAudioObject(OwnerID());
	}

	inline SFBCFString Name() const
	{
		return CFTypeProperty<CFStringRef>(SFBAudioObjectPropertyAddress(kAudioObjectPropertyName));
	}

	inline SFBCFString ModelName() const
	{
		return CFTypeProperty<CFStringRef>(SFBAudioObjectPropertyAddress(kAudioObjectPropertyModelName));
	}

	inline SFBCFString Manufacturer() const
	{
		return CFTypeProperty<CFStringRef>(SFBAudioObjectPropertyAddress(kAudioObjectPropertyManufacturer));
	}

	inline SFBCFString ElementName(AudioObjectPropertyScope inScope, AudioObjectPropertyElement inElement) const
	{
		return CFTypeProperty<CFStringRef>(SFBAudioObjectPropertyAddress(kAudioObjectPropertyElementName, inScope, inElement));
	}

	inline SFBCFString ElementCategoryName(AudioObjectPropertyScope inScope, AudioObjectPropertyElement inElement) const
	{
		return CFTypeProperty<CFStringRef>(SFBAudioObjectPropertyAddress(kAudioObjectPropertyElementCategoryName, inScope, inElement));
	}

	inline SFBCFString ElementNumberName(AudioObjectPropertyScope inScope, AudioObjectPropertyElement inElement) const
	{
		return CFTypeProperty<CFStringRef>(SFBAudioObjectPropertyAddress(kAudioObjectPropertyElementNumberName, inScope, inElement));
	}

	inline std::vector<AudioObjectID> OwnedObjectIDs() const
	{
		return ArrayProperty<AudioObjectID>(SFBAudioObjectPropertyAddress(kAudioObjectPropertyOwnedObjects));
	}

	std::vector<SFBHALAudioObject> OwnedObjects() const
	{
		auto vec = OwnedObjectIDs();
		std::vector<SFBHALAudioObject> result(vec.size());
		std::transform(vec.cbegin(), vec.cend(), result.begin(), [](AudioObjectID objectID) { return SFBHALAudioObject(objectID); });
		return result;
	}

//	kAudioObjectPropertyIdentify            = 'iden',

	inline SFBCFString SerialNumber() const
	{
		return CFTypeProperty<CFStringRef>(SFBAudioObjectPropertyAddress(kAudioObjectPropertySerialNumber));
	}

	inline SFBCFString FirmwareVersion() const
	{
		return CFTypeProperty<CFStringRef>(SFBAudioObjectPropertyAddress(kAudioObjectPropertyFirmwareVersion));
	}

protected:

	// The underlying object ID
	AudioObjectID mObjectID;

};
