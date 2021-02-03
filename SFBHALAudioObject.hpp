//
// Copyright (c) 2021 Stephen F. Booth <me@sbooth.org>
// MIT license
//

#pragma once

#import <vector>

#import <CoreAudio/CoreAudio.h>

#import "SFBAudioObjectPropertyAddress.hpp"
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
	inline SFBHALAudioObject() noexcept
	: mObjectID(kAudioObjectUnknown)
	{}

	/// Copy constructor
	inline SFBHALAudioObject(const SFBHALAudioObject& rhs) noexcept
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
	inline SFBHALAudioObject(AudioObjectID objectID) noexcept
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
		return mObjectID == kAudioObjectUnknown;
	}

	/// Returns @c true if @c rhs is equal to @c this
	inline bool operator==(AudioObjectID rhs) const noexcept
	{
		return mObjectID == rhs;
	}

	/// Returns @c true if @c rhs is not equal to @c this
	inline bool operator!=(AudioObjectID rhs) const noexcept
	{
		return mObjectID != rhs;
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

	bool HasProperty(const AudioObjectPropertyAddress& inAddress) const noexcept;
	bool IsPropertySettable(const AudioObjectPropertyAddress& inAddress) const;
	UInt32 GetPropertyDataSize(const AudioObjectPropertyAddress& inAddress, UInt32 inQualifierDataSize = 0, const void * _Nullable inQualifierData = nullptr) const;

	void GetPropertyData(const AudioObjectPropertyAddress& inAddress, UInt32 inQualifierDataSize, const void * _Nullable inQualifierData, UInt32& ioDataSize, void * _Nonnull outData) const;
	void SetPropertyData(const AudioObjectPropertyAddress& inAddress, UInt32 inQualifierDataSize, const void * _Nullable inQualifierData, UInt32 inDataSize, const void * _Nonnull inData);

	template <typename T>
	typename std::enable_if<std::is_integral<T>::value, bool>::type IntegralProperty(const AudioObjectPropertyAddress& inAddress, UInt32 inQualifierDataSize = 0, const void * _Nullable inQualifierData = nullptr) const
	{
		T value;
		UInt32 size = sizeof(T);
		GetPropertyData(inAddress, inQualifierDataSize, inQualifierData, size, &value);
		return value;
	}

	template <typename T>
	typename std::enable_if<std::is_trivial<T>::value, bool>::type StructProperty(const AudioObjectPropertyAddress& inAddress, UInt32 inQualifierDataSize = 0, const void * _Nullable inQualifierData = nullptr) const
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

	void AddPropertyListener(const AudioObjectPropertyAddress& inAddress, AudioObjectPropertyListenerProc _Nonnull inListenerProc, void * _Nullable inClientData);
	void RemovePropertyListener(const AudioObjectPropertyAddress& inAddress, AudioObjectPropertyListenerProc _Nonnull inListenerProc, void * _Nullable inClientData);

	void AddPropertyListenerBlock(const AudioObjectPropertyAddress& inAddress, dispatch_queue_t _Nonnull inDispatchQueue, AudioObjectPropertyListenerBlock _Nonnull inListenerBlock);
	void RemovePropertyListenerBlock(const AudioObjectPropertyAddress& inAddress, dispatch_queue_t _Nonnull inDispatchQueue, AudioObjectPropertyListenerBlock _Nonnull inListenerBlock);

#pragma mark AudioObject Properties

	inline AudioClassID BaseClass() const
	{
		return IntegralProperty<AudioClassID>(SFBAudioObjectPropertyAddress(kAudioObjectPropertyBaseClass));
	}

	inline AudioClassID Class() const
	{
		return IntegralProperty<AudioClassID>(SFBAudioObjectPropertyAddress(kAudioObjectPropertyClass));
	}

	inline AudioObjectID OwnerID() const
	{
		return IntegralProperty<AudioObjectID>(SFBAudioObjectPropertyAddress(kAudioObjectPropertyOwner));
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
