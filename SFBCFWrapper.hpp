//
// Copyright (c) 2012 - 2021 Stephen F. Booth <me@sbooth.org>
// MIT license
//

#pragma once

#import <CFNetwork/CFNetwork.h>
#import <CoreFoundation/CoreFoundation.h>
#if !TARGET_OS_IPHONE
#import <ImageIO/ImageIO.h>
#import <Security/Security.h>
#endif

/// A wrapper around a Core Foundation object
///
/// @c SFBCFWrapper simplifies the use of CFTypes in C++ by wrapping a CF object, ensuring
/// @c CFRelease will be called when the @c SFBCFWrapper goes out of scope.
/// @tparam T A @c CFType
template <typename T>
class SFBCFWrapper
{
	
public:

#pragma mark Creation and Destruction

	/// Creates a new @c SFBCFWrapper
	inline SFBCFWrapper()
	: SFBCFWrapper(nullptr)
	{}

	/// Creates a new @c SFBCFWrapper
	SFBCFWrapper(const SFBCFWrapper& rhs)
	: mObject(rhs.mObject), mRelease(rhs.mRelease)
	{
		if(mObject && mRelease)
			CFRetain(mObject);
	}

	/// Replaces the wrapped object
	SFBCFWrapper& operator=(const SFBCFWrapper& rhs)
	{
		if(mObject != rhs.mObject) {
			if(mObject && mRelease)
				CFRelease(mObject);

			mObject = rhs.mObject;
			mRelease = rhs.mRelease;

			if(mObject && mRelease)
				CFRetain(mObject);
		}

		return *this;
	}

	/// Destroys this @c SFBCFWrapper and ensure @c CFRelease() is called if necessary
	~SFBCFWrapper()
	{
		if(mObject && mRelease)
			CFRelease(mObject);
		mObject = nullptr;
	}

	/// Creates a new @c SFBCFWrapper
	SFBCFWrapper(SFBCFWrapper&& rhs)
	: mObject(rhs.mObject), mRelease(rhs.mRelease)
	{
		rhs.mObject = nullptr;
	}

	/// Replaces the wrapped object
	SFBCFWrapper& operator=(SFBCFWrapper&& rhs)
	{
		if(mObject != rhs.mObject) {
			if(mObject && mRelease)
				CFRelease(mObject);

			mObject = rhs.mObject;
			mRelease = rhs.mRelease;

			rhs.mObject = nullptr;
		}

		return *this;
	}


	/// Create a new @c SFBCFWrapper
	/// @note The @c SFBCFWrapper takes ownership of @c object
	/// @param object The object to wrap
	inline explicit SFBCFWrapper(T object)
	: SFBCFWrapper(object, true)
	{}

	/// Creates a new @c SFBCFWrapper
	/// @param object The object to wrap
	/// @param release Whether this @c SFBCFWrapper should take ownership of @c object
	SFBCFWrapper(T object, bool release)
	: mObject(object), mRelease(release)
	{}

#pragma mark Assignment

	/// Replaces the wrapped object
	/// @note The @c SFBCFWrapper takes ownership of @c rhs
	/// @param rhs The object to wrap
	SFBCFWrapper& operator=(const T& rhs)
	{
		if(mObject != rhs) {
			if(mObject && mRelease)
				CFRelease(mObject);

			mObject = rhs;
			mRelease = true;
		}

		return *this;
	}

#pragma mark Pointer management

	/// Relinquishes ownership of the wrapped object and returns it
	inline T Relinquish()
	{
		T object = mObject;
		mObject = nullptr;

		return object;
	}

#pragma mark Equality testing

	/// Tests two @c SFBCFWrapper objects for equality using @c CFEqual()
	inline bool operator==(const SFBCFWrapper& rhs) const
	{
		if(mObject == rhs.mObject)
			return true;

		// CFEqual doesn't handle nullptr
		if(!mObject || !rhs.mObject)
			return false;

		return CFEqual(mObject, rhs.mObject);
	}

	/// Tests two @c SFBCFWrapper objects for inequality
	inline bool operator!=(const SFBCFWrapper& rhs) const
	{
		return !operator==(rhs);
	}

#pragma mark CoreFoundation object access

	/// Returns @c true if the wrapped object is not @c nullptr
	inline explicit operator bool() const
	{
		return mObject != nullptr;
	}

	/// Returns @c true if the wrapped object is @c nullptr
	inline bool operator!() const
	{
		return !operator bool();
	}

	/// Returns the wrapped object
	inline operator T() const
	{
		return mObject;
	}


	/// Returns a pointer to the wrapped object
	inline T * operator&()
	{
		return &mObject;
	}


	/// Returns the wrapped object
	inline T Object() const
	{
		return mObject;
	}

#pragma mark CoreFoundation object creation

	/// Creates a new wrapped @c CFStringRef using @c CFStringCreateWithCString with the default allocator
	template <typename = std::enable_if<std::is_same<T, CFStringRef>::value>>
	SFBCFWrapper(const char *cStr, CFStringEncoding encoding)
	: SFBCFWrapper(CFStringCreateWithCString(kCFAllocatorDefault, cStr, encoding))
	{}

	/// Creates a new wrapped @c CFStringRef using @c CFStringCreateWithFormatAndArguments with the default allocator
	template <typename = std::enable_if<std::is_same<T, CFStringRef>::value>>
	SFBCFWrapper(CFDictionaryRef formatOptions, CFStringRef format, ...) CF_FORMAT_FUNCTION(3,4)
	: SFBCFWrapper()
	{
		va_list ap;
		va_start(ap, format);
		*this = CFStringCreateWithFormatAndArguments(kCFAllocatorDefault, formatOptions, format, ap);
		va_end(ap);
	}

	/// Creates a new wrapped @c CFNumberRef using @c CFNumberCreate with the default allocator
	template <typename = std::enable_if<std::is_same<T, CFNumberRef>::value>>
	SFBCFWrapper(CFNumberType theType, const void *valuePtr)
	: SFBCFWrapper(CFNumberCreate(kCFAllocatorDefault, theType, valuePtr))
	{}

	/// Creates a new wrapped @c CFArrayRef using @c CFArrayCreate with the default allocator
	template <typename = std::enable_if<std::is_same<T, CFArrayRef>::value>>
	SFBCFWrapper(const void **values, CFIndex numValues, const CFArrayCallBacks *callBacks)
	: SFBCFWrapper(CFArrayCreate(kCFAllocatorDefault, values, numValues, callBacks))
	{}

	/// Creates a new wrapped @c CFMutableArrayRef using @c CFArrayCreateMutable with the default allocator
	template <typename = std::enable_if<std::is_same<T, CFMutableArrayRef>::value>>
	SFBCFWrapper(CFIndex capacity, const CFArrayCallBacks *callBacks)
	: SFBCFWrapper(CFArrayCreateMutable(kCFAllocatorDefault, capacity, callBacks))
	{}

	/// Creates a new wrapped @c CFDictionaryRef using @c CFDictionaryCreate with the default allocator
	template <typename = std::enable_if<std::is_same<T, CFDictionaryRef>::value>>
	SFBCFWrapper(const void **keys, const void **values, CFIndex numValues, const CFDictionaryKeyCallBacks *keyCallBacks, const CFDictionaryValueCallBacks *valueCallBacks)
	: SFBCFWrapper(CFDictionaryCreate(kCFAllocatorDefault, keys, values, numValues, keyCallBacks, valueCallBacks))
	{}

	/// Creates a new wrapped @c CFMutableDictionaryRef using @c CFDictionaryCreateMutable with the default allocator
	template <typename = std::enable_if<std::is_same<T, CFMutableDictionaryRef>::value>>
	SFBCFWrapper(CFIndex capacity, const CFDictionaryKeyCallBacks *keyCallBacks, const CFDictionaryValueCallBacks *valueCallBacks)
	: SFBCFWrapper(CFDictionaryCreateMutable(kCFAllocatorDefault, capacity, keyCallBacks, valueCallBacks))
	{}

	/// Creates a new wrapped @c CFDataRef using @c CFDataCreate with the default allocator
	template <typename = std::enable_if<std::is_same<T, CFDataRef>::value>>
	SFBCFWrapper(const UInt8 *bytes, CFIndex length)
	: SFBCFWrapper(CFDataCreate(kCFAllocatorDefault, bytes, length))
	{}

private:

	/// The Core Foundation object
	T mObject;
	/// Whether @c CFRelease should be called on destruction or reassignment
	bool mRelease;

};

// ========================================
// Typedefs for common CoreFoundation types

/// A wrapped @c CFTypeRef
using SFBCFType = SFBCFWrapper<CFTypeRef>;
/// A wrapped @c CFDataRef
using SFBCFData = SFBCFWrapper<CFDataRef>;
/// A wrapped @c CFMutableDataRef
using SFBCFMutableData = SFBCFWrapper<CFMutableDataRef>;
/// A wrapped @c CFStringRef
using SFBCFString = SFBCFWrapper<CFStringRef>;
/// A wrapped @c CFMutableStringRef
using SFBCFMutableString = SFBCFWrapper<CFMutableStringRef>;
/// A wrapped @c CFAttributedStringRef
using SFBCFAttributedString = SFBCFWrapper<CFAttributedStringRef>;
/// A wrapped @c CFMutableAttributedStringRef
using SFBCFMutableAttributedString = SFBCFWrapper<CFMutableAttributedStringRef>;
/// A wrapped @c CFDictionaryRef
using SFBCFDictionary = SFBCFWrapper<CFDictionaryRef>;
/// A wrapped @c CFMutableDictionaryRef
using SFBCFMutableDictionary = SFBCFWrapper<CFMutableDictionaryRef>;
/// A wrapped @c CFArrayRef
using SFBCFArray = SFBCFWrapper<CFArrayRef>;
/// A wrapped @c CFMutableArrayRef
using SFBCFMutableArray = SFBCFWrapper<CFMutableArrayRef>;
/// A wrapped @c CFSetRef
using SFBCFSet = SFBCFWrapper<CFSetRef>;
/// A wrapped @c CFMutableSetRef
using SFBCFMutableSet = SFBCFWrapper<CFMutableSetRef>;
/// A wrapped @c CFBagRef
using SFBCFBag = SFBCFWrapper<CFBagRef>;
/// A wrapped @c CFMutableBagRef
using SFBCFMutableBag = SFBCFWrapper<CFMutableBagRef>;
/// A wrapped @c CFPropertyListRef
using SFBCFPropertyList = SFBCFWrapper<CFPropertyListRef>;
/// A wrapped @c CFBitVectorRef
using SFBCFBitVector = SFBCFWrapper<CFBitVectorRef>;
/// A wrapped @c CFMutableBitVectorRef
using SFBCFMutableBitVector = SFBCFWrapper<CFMutableBitVectorRef>;
/// A wrapped @c CFCharacterSetRef
using SFBCFCharacterSet = SFBCFWrapper<CFCharacterSetRef>;
/// A wrapped @c CFMutableCharacterSetRef
using SFBCFMutableCharacterSet = SFBCFWrapper<CFMutableCharacterSetRef>;
/// A wrapped @c CFURLRef
using SFBCFURL = SFBCFWrapper<CFURLRef>;
/// A wrapped @c CFUUIDRef
using SFBCFUUID = SFBCFWrapper<CFUUIDRef>;
/// A wrapped @c CFNumberRef
using SFBCFNumber = SFBCFWrapper<CFNumberRef>;
/// A wrapped @c CFBooleanRef
using SFBCFBoolean = SFBCFWrapper<CFBooleanRef>;
/// A wrapped @c CFErrorRef
using SFBCFError = SFBCFWrapper<CFErrorRef>;
/// A wrapped @c CFDateRef
using SFBCFDate = SFBCFWrapper<CFDateRef>;
/// A wrapped @c CFReadStreamRef
using SFBCFReadStream = SFBCFWrapper<CFReadStreamRef>;
/// A wrapped @c CFWriteStreamRef
using SFBCFWriteStream = SFBCFWrapper<CFWriteStreamRef>;
/// A wrapped @c CFHTTPMessageRef
using SFBCFHTTPMessage = SFBCFWrapper<CFHTTPMessageRef>;
#if !TARGET_OS_IPHONE
/// A wrapped @c SecKeychainItemRef
using SFBSecKeychainItem = SFBCFWrapper<SecKeychainItemRef>;
/// A wrapped @c SecCertificateRef
using SFBSecCertificate = SFBCFWrapper<SecCertificateRef>;
/// A wrapped @c SecTransformRef
using SFBSecTransform = SFBCFWrapper<SecTransformRef>;
/// A wrapped @c CGImageSourceRef
using SFBCGImageSource = SFBCFWrapper<CGImageSourceRef>;
#endif
