/*
 * Copyright (c) 2012 - 2021 Stephen F. Booth <me@sbooth.org>
 * MIT license
 */

#pragma once

#import <CFNetwork/CFNetwork.h>
#import <CoreFoundation/CoreFoundation.h>
#if !TARGET_OS_IPHONE
#import <ImageIO/ImageIO.h>
#import <Security/Security.h>
#endif

/*!
 * A wrapper around a Core Foundation object
 *
 * @c SFBCFWrapper simplifies the use of CFTypes in C++ by wrapping a CF object, ensuring
 * @c CFRelease will be called when the @c SFBCFWrapper goes out of scope.
 * @tparam T A @c CFType
 */
template <typename T>
class SFBCFWrapper
{
	
public:

#pragma mark Creation and Destruction

	/*! Create a new @c SFBCFWrapper */
	inline SFBCFWrapper()
	: SFBCFWrapper(nullptr)
	{}

	/*!
	 * Create a new @c SFBCFWrapper
	 * @note The @c SFBCFWrapper takes ownership of @c object
	 * @param object The object to wrap
	 */
	inline explicit SFBCFWrapper(T object)
	: SFBCFWrapper(object, true)
	{}
	
	/*!
	 * Create a new @c SFBCFWrapper
	 * @param object The object to wrap
	 * @param release Whether this @c SFBCFWrapper should take ownership of @c object
	 */
	SFBCFWrapper(T object, bool release)
	: mObject(object), mRelease(release)
	{}


	/*! Create a new @c SFBCFWrapper */
	SFBCFWrapper(SFBCFWrapper&& rhs)
	: mObject(rhs.mObject), mRelease(rhs.mRelease)
	{
		rhs.mObject = nullptr;
	}

	/*! Create a new @c SFBCFWrapper */
	SFBCFWrapper(const SFBCFWrapper& rhs)
	: mObject(rhs.mObject), mRelease(rhs.mRelease)
	{
		if(mObject && mRelease)
			CFRetain(mObject);
	}

	/*! Destroy this @c SFBCFWrapper and ensure @c CFRelease() is called if necessary */
	~SFBCFWrapper()
	{
		if(mObject && mRelease)
			CFRelease(mObject);
		mObject = nullptr;
	}

#pragma mark Assignment

	/*!
	 * Replace the wrapped object
	 * @note The @c SFBCFWrapper takes ownership of @c rhs
	 * @param rhs The object to wrap
	 */
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

	/*! Replace the wrapped object */
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

	/*! Replace the wrapped object */
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

#pragma mark Pointer management

	/*! Relinquish ownership of the wrapped object and return it */
	inline T Relinquish()
	{
		T object = mObject;
		mObject = nullptr;

		return object;
	}

#pragma mark Equality testing

	/*! Test two @c SFBCFWrapper objects for equality using @c CFEqual() */
	inline bool operator==(const SFBCFWrapper& rhs) const
	{
		if(mObject == rhs.mObject)
			return true;

		// CFEqual doesn't handle nullptr
		if(!mObject || !rhs.mObject)
			return false;

		return CFEqual(mObject, rhs.mObject);
	}

	/*! Test two @c SFBCFWrapper objects for inequality */
	inline bool operator!=(const SFBCFWrapper& rhs) const
	{
		return !operator==(rhs);
	}

#pragma mark CoreFoundation object access

	/*! Check whether the wrapped object is not @c nullptr */
	inline explicit operator bool() const
	{
		return nullptr != mObject;
	}

	/*! Check whether the wrapped object is @c nullptr */
	inline bool operator!() const
	{
		return nullptr == mObject;
	}

	/*! Get the wrapped object */
	inline operator T() const
	{
		return mObject;
	}


	/*! Get a pointer to the wrapped object */
	inline T * operator&()
	{
		return &mObject;
	}


	/*! Get the wrapped object */
	inline T Object() const
	{
		return mObject;
	}

#pragma mark CoreFoundation object creation

	/*! Create a new wrapped @c CFStringRef using @c CFStringCreateWithCString with the default allocator */
	template <typename = std::enable_if<std::is_same<T, CFStringRef>::value>>
	SFBCFWrapper(const char *cStr, CFStringEncoding encoding)
	: SFBCFWrapper(CFStringCreateWithCString(kCFAllocatorDefault, cStr, encoding))
	{}

	/*! Create a new wrapped @c CFStringRef using @c CFStringCreateWithFormatAndArguments with the default allocator */
	template <typename = std::enable_if<std::is_same<T, CFStringRef>::value>>
	SFBCFWrapper(CFDictionaryRef formatOptions, CFStringRef format, ...) CF_FORMAT_FUNCTION(3,4)
	: SFBCFWrapper()
	{
		va_list ap;
		va_start(ap, format);
		*this = CFStringCreateWithFormatAndArguments(kCFAllocatorDefault, formatOptions, format, ap);
		va_end(ap);
	}

	/*! Create a new wrapped @c CFNumberRef using @c CFNumberCreate with the default allocator */
	template <typename = std::enable_if<std::is_same<T, CFNumberRef>::value>>
	SFBCFWrapper(CFNumberType theType, const void *valuePtr)
	: SFBCFWrapper(CFNumberCreate(kCFAllocatorDefault, theType, valuePtr))
	{}

	/*! Create a new wrapped @c CFArrayRef using @c CFArrayCreate with the default allocator */
	template <typename = std::enable_if<std::is_same<T, CFArrayRef>::value>>
	SFBCFWrapper(const void **values, CFIndex numValues, const CFArrayCallBacks *callBacks)
	: SFBCFWrapper(CFArrayCreate(kCFAllocatorDefault, values, numValues, callBacks))
	{}

	/*! Create a new wrapped @c CFMutableArrayRef using @c CFArrayCreateMutable with the default allocator */
	template <typename = std::enable_if<std::is_same<T, CFMutableArrayRef>::value>>
	SFBCFWrapper(CFIndex capacity, const CFArrayCallBacks *callBacks)
	: SFBCFWrapper(CFArrayCreateMutable(kCFAllocatorDefault, capacity, callBacks))
	{}

	/*! Create a new wrapped @c CFDictionaryRef using @c CFDictionaryCreate with the default allocator */
	template <typename = std::enable_if<std::is_same<T, CFDictionaryRef>::value>>
	SFBCFWrapper(const void **keys, const void **values, CFIndex numValues, const CFDictionaryKeyCallBacks *keyCallBacks, const CFDictionaryValueCallBacks *valueCallBacks)
	: SFBCFWrapper(CFDictionaryCreate(kCFAllocatorDefault, keys, values, numValues, keyCallBacks, valueCallBacks))
	{}

	/*! Create a new wrapped @c CFMutableDictionaryRef using @c CFDictionaryCreateMutable with the default allocator */
	template <typename = std::enable_if<std::is_same<T, CFMutableDictionaryRef>::value>>
	SFBCFWrapper(CFIndex capacity, const CFDictionaryKeyCallBacks *keyCallBacks, const CFDictionaryValueCallBacks *valueCallBacks)
	: SFBCFWrapper(CFDictionaryCreateMutable(kCFAllocatorDefault, capacity, keyCallBacks, valueCallBacks))
	{}

	/*! Create a new wrapped @c CFDataRef using @c CFDataCreate with the default allocator */
	template <typename = std::enable_if<std::is_same<T, CFDataRef>::value>>
	SFBCFWrapper(const UInt8 *bytes, CFIndex length)
	: SFBCFWrapper(CFDataCreate(kCFAllocatorDefault, bytes, length))
	{}

private:

	T mObject;				/*!< The Core Foundation object */
	bool mRelease;			/*!< Whether @c CFRelease should be called on destruction or reassignment */

};

// ========================================
// Typedefs for common CoreFoundation types

using SFBCFType 					= SFBCFWrapper<CFTypeRef>;						/*!< A wrapped @c CFTypeRef */
using SFBCFData 					= SFBCFWrapper<CFDataRef>;						/*!< A wrapped @c CFDataRef */
using SFBCFMutableData 				= SFBCFWrapper<CFMutableDataRef>;				/*!< A wrapped @c CFMutableDataRef */
using SFBCFString 					= SFBCFWrapper<CFStringRef>;					/*!< A wrapped @c CFStringRef */
using SFBCFMutableString 			= SFBCFWrapper<CFMutableStringRef>;				/*!< A wrapped @c CFMutableStringRef */
using SFBCFAttributedString 		= SFBCFWrapper<CFAttributedStringRef>;			/*!< A wrapped @c CFAttributedStringRef */
using SFBCFMutableAttributedString 	= SFBCFWrapper<CFMutableAttributedStringRef>; 	/*!< A wrapped @c CFMutableAttributedStringRef */
using SFBCFDictionary 				= SFBCFWrapper<CFDictionaryRef>;				/*!< A wrapped @c CFDictionaryRef */
using SFBCFMutableDictionary 		= SFBCFWrapper<CFMutableDictionaryRef>;			/*!< A wrapped @c CFMutableDictionaryRef */
using SFBCFArray 					= SFBCFWrapper<CFArrayRef>;						/*!< A wrapped @c CFArrayRef */
using SFBCFMutableArray 			= SFBCFWrapper<CFMutableArrayRef>;				/*!< A wrapped @c CFMutableArrayRef */
using SFBCFSet 						= SFBCFWrapper<CFSetRef>;						/*!< A wrapped @c CFSetRef */
using SFBCFMutableSet 				= SFBCFWrapper<CFMutableSetRef>;				/*!< A wrapped @c CFMutableSetRef */
using SFBCFBag 						= SFBCFWrapper<CFBagRef>;						/*!< A wrapped @c CFBagRef */
using SFBCFMutableBag 				= SFBCFWrapper<CFMutableBagRef>;				/*!< A wrapped @c CFMutableBagRef */
using SFBCFPropertyList 			= SFBCFWrapper<CFPropertyListRef>;				/*!< A wrapped @c CFPropertyListRef */
using SFBCFBitVector 				= SFBCFWrapper<CFBitVectorRef>;					/*!< A wrapped @c CFBitVectorRef */
using SFBCFMutableBitVector 		= SFBCFWrapper<CFMutableBitVectorRef>;			/*!< A wrapped @c CFMutableBitVectorRef */
using SFBCFCharacterSet 			= SFBCFWrapper<CFCharacterSetRef>;				/*!< A wrapped @c CFCharacterSetRef */
using SFBCFMutableCharacterSet 		= SFBCFWrapper<CFMutableCharacterSetRef>;		/*!< A wrapped @c CFMutableCharacterSetRef */
using SFBCFURL 						= SFBCFWrapper<CFURLRef>;						/*!< A wrapped @c CFURLRef */
using SFBCFUUID 					= SFBCFWrapper<CFUUIDRef>;						/*!< A wrapped @c CFUUIDRef */
using SFBCFNumber 					= SFBCFWrapper<CFNumberRef>;					/*!< A wrapped @c CFNumberRef */
using SFBCFBoolean 					= SFBCFWrapper<CFBooleanRef>;					/*!< A wrapped @c CFBooleanRef */
using SFBCFError 					= SFBCFWrapper<CFErrorRef>;						/*!< A wrapped @c CFErrorRef */
using SFBCFDate 					= SFBCFWrapper<CFDateRef>;						/*!< A wrapped @c CFDateRef */
using SFBCFReadStream 				= SFBCFWrapper<CFReadStreamRef>;				/*!< A wrapped @c CFReadStream */
using SFBCFWriteStream 				= SFBCFWrapper<CFWriteStreamRef>;				/*!< A wrapped @c CFWriteStream */
using SFBCFHTTPMessage 				= SFBCFWrapper<CFHTTPMessageRef>;				/*!< A wrapped @c CFHTTPMessageRef */
#if !TARGET_OS_IPHONE
using SFBSecKeychainItem 			= SFBCFWrapper<SecKeychainItemRef>;				/*!< A wrapped @c SecKeychainItemRef */
using SFBSecCertificate 			= SFBCFWrapper<SecCertificateRef>;				/*!< A wrapped @c SecCertificateRef */
using SFBSecTransform 				= SFBCFWrapper<SecTransformRef>;				/*!< A wrapped @c SecTransformRef */
using SFBCGImageSource 				= SFBCFWrapper<CGImageSourceRef>;				/*!< A wrapped @c CGImageSourceRef */
#endif
