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
 * @brief A wrapper around a Core Foundation object
 *
 * \c SFBCFWrapper simplifies the use of CFTypes in C++ by wrapping a CF object, ensuring
 * \c CFRelease will be called when the \c SFBCFWrapper goes out of scope.
 * @tparam T A \c CFType
 */
template <typename T>
class SFBCFWrapper
{
	
public:

	// ========================================
	/*! @name Creation and Destruction */
	//@{

	/*! @brief Create a new \c SFBCFWrapper */
	inline SFBCFWrapper()
	: SFBCFWrapper(nullptr)
	{}

	/*!
	 * @brief Create a new \c SFBCFWrapper
	 * @note The \c SFBCFWrapper takes ownership of \c object
	 * @param object The object to wrap
	 */
	inline explicit SFBCFWrapper(T object)
	: SFBCFWrapper(object, true)
	{}
	
	/*!
	 * @brief Create a new \c SFBCFWrapper
	 * @param object The object to wrap
	 * @param release Whether this \c SFBCFWrapper should take ownership of \c object
	 */
	SFBCFWrapper(T object, bool release)
	: mObject(object), mRelease(release)
	{}


	/*! @brief Create a new \c SFBCFWrapper */
	SFBCFWrapper(SFBCFWrapper&& rhs)
	: mObject(rhs.mObject), mRelease(rhs.mRelease)
	{
		rhs.mObject = nullptr;
	}

	/*! @brief Create a new \c SFBCFWrapper */
	SFBCFWrapper(const SFBCFWrapper& rhs)
	: mObject(rhs.mObject), mRelease(rhs.mRelease)
	{
		if(mObject && mRelease)
			CFRetain(mObject);
	}

	/*! @brief Destroy this \c SFBCFWrapper and ensure \c CFRelease() is called if necessary */
	~SFBCFWrapper()
	{
		if(mObject && mRelease)
			CFRelease(mObject);
		mObject = nullptr;
	}

	//@}


	/*! @name Assignment */
	//@{

	/*!
	 * @brief Replace the wrapped object
	 * @note The \c SFBCFWrapper takes ownership of \c rhs
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

	/*! @brief Replace the wrapped object */
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

	/*! @brief Replace the wrapped object */
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

	//@}


	// ========================================
	/*! @name Pointer management */
	//@{

	/*! @brief Relinquish ownership of the wrapped object and return it */
	inline T Relinquish()
	{
		T object = mObject;
		mObject = nullptr;

		return object;
	}

	//@}


	// ========================================
	/*! @name Equality testing */
	//@{

	/*! @brief Test two \c SFBCFWrapper objects for equality using \c CFEqual() */
	inline bool operator==(const SFBCFWrapper& rhs) const
	{
		if(mObject == rhs.mObject)
			return true;

		// CFEqual doesn't handle nullptr
		if(!mObject || !rhs.mObject)
			return false;

		return CFEqual(mObject, rhs.mObject);
	}

	/*! @brief Test two \c SFBCFWrapper objects for inequality */
	inline bool operator!=(const SFBCFWrapper& rhs) const
	{
		return !operator==(rhs);
	}

	//@}


	// ========================================
	/*! @name CoreFoundation object access */
	//@{

	/*! @brief Check whether the wrapped object is not \c nullptr */
	inline explicit operator bool() const
	{
		return nullptr != mObject;
	}

	/*! @brief Check whether the wrapped object is \c nullptr */
	inline bool operator!() const
	{
		return nullptr == mObject;
	}

	/*! @brief Get the wrapped object */
	inline operator T() const
	{
		return mObject;
	}


	/*! @brief Get a pointer to the wrapped object */
	inline T * operator&()
	{
		return &mObject;
	}


	/*! @brief Get the wrapped object */
	inline T Object() const
	{
		return mObject;
	}

	//@}


	// ========================================
	/*! @name CoreFoundation object creation */
	//@{

	/*! @brief Create a new wrapped \c CFStringRef using \c CFStringCreateWithCString with the default allocator */
	template <typename = std::enable_if<std::is_same<T, CFStringRef>::value>>
	SFBCFWrapper(const char *cStr, CFStringEncoding encoding)
	: SFBCFWrapper(CFStringCreateWithCString(kCFAllocatorDefault, cStr, encoding))
	{}

	/*! @brief Create a new wrapped \c CFStringRef using \c CFStringCreateWithFormatAndArguments with the default allocator */
	template <typename = std::enable_if<std::is_same<T, CFStringRef>::value>>
	SFBCFWrapper(CFDictionaryRef formatOptions, CFStringRef format, ...) CF_FORMAT_FUNCTION(3,4)
	: SFBCFWrapper()
	{
		va_list ap;
		va_start(ap, format);
		*this = CFStringCreateWithFormatAndArguments(kCFAllocatorDefault, formatOptions, format, ap);
		va_end(ap);
	}

	/*! @brief Create a new wrapped \c CFNumberRef using \c CFNumberCreate with the default allocator */
	template <typename = std::enable_if<std::is_same<T, CFNumberRef>::value>>
	SFBCFWrapper(CFNumberType theType, const void *valuePtr)
	: SFBCFWrapper(CFNumberCreate(kCFAllocatorDefault, theType, valuePtr))
	{}

	/*! @brief Create a new wrapped \c CFArrayRef using \c CFArrayCreate with the default allocator */
	template <typename = std::enable_if<std::is_same<T, CFArrayRef>::value>>
	SFBCFWrapper(const void **values, CFIndex numValues, const CFArrayCallBacks *callBacks)
	: SFBCFWrapper(CFArrayCreate(kCFAllocatorDefault, values, numValues, callBacks))
	{}

	/*! @brief Create a new wrapped \c CFMutableArrayRef using \c CFArrayCreateMutable with the default allocator */
	template <typename = std::enable_if<std::is_same<T, CFMutableArrayRef>::value>>
	SFBCFWrapper(CFIndex capacity, const CFArrayCallBacks *callBacks)
	: SFBCFWrapper(CFArrayCreateMutable(kCFAllocatorDefault, capacity, callBacks))
	{}

	/*! @brief Create a new wrapped \c CFDictionaryRef using \c CFDictionaryCreate with the default allocator */
	template <typename = std::enable_if<std::is_same<T, CFDictionaryRef>::value>>
	SFBCFWrapper(const void **keys, const void **values, CFIndex numValues, const CFDictionaryKeyCallBacks *keyCallBacks, const CFDictionaryValueCallBacks *valueCallBacks)
	: SFBCFWrapper(CFDictionaryCreate(kCFAllocatorDefault, keys, values, numValues, keyCallBacks, valueCallBacks))
	{}

	/*! @brief Create a new wrapped \c CFMutableDictionaryRef using \c CFDictionaryCreateMutable with the default allocator */
	template <typename = std::enable_if<std::is_same<T, CFMutableDictionaryRef>::value>>
	SFBCFWrapper(CFIndex capacity, const CFDictionaryKeyCallBacks *keyCallBacks, const CFDictionaryValueCallBacks *valueCallBacks)
	: SFBCFWrapper(CFDictionaryCreateMutable(kCFAllocatorDefault, capacity, keyCallBacks, valueCallBacks))
	{}

	/*! @brief Create a new wrapped \c CFDataRef using \c CFDataCreate with the default allocator */
	template <typename = std::enable_if<std::is_same<T, CFDataRef>::value>>
	SFBCFWrapper(const UInt8 *bytes, CFIndex length)
	: SFBCFWrapper(CFDataCreate(kCFAllocatorDefault, bytes, length))
	{}

	//@}

private:
	T mObject;				/*!< The Core Foundation object */
	bool mRelease;			/*!< Whether \c CFRelease should be called on destruction or reassignment */
};

// ========================================
// Typedefs for common CoreFoundation types

using SFBCFType = SFBCFWrapper<CFTypeRef>;											/*!< @brief A wrapped \c CFTypeRef */
using SFBCFData = SFBCFWrapper<CFDataRef>;											/*!< @brief A wrapped \c CFDataRef */
using SFBCFMutableData = SFBCFWrapper<CFMutableDataRef>;							/*!< @brief A wrapped \c CFMutableDataRef */
using SFBCFString = SFBCFWrapper<CFStringRef>;										/*!< @brief A wrapped \c CFStringRef */
using SFBCFMutableString = SFBCFWrapper<CFMutableStringRef>;						/*!< @brief A wrapped \c CFMutableStringRef */
using SFBCFAttributedString = SFBCFWrapper<CFAttributedStringRef>;					/*!< @brief A wrapped \c CFAttributedStringRef */
using SFBCFMutableAttributedString = SFBCFWrapper<CFMutableAttributedStringRef>; 	/*!< @brief A wrapped \c CFMutableAttributedStringRef */
using SFBCFDictionary = SFBCFWrapper<CFDictionaryRef>;								/*!< @brief A wrapped \c CFDictionaryRef */
using SFBCFMutableDictionary = SFBCFWrapper<CFMutableDictionaryRef>;				/*!< @brief A wrapped \c CFMutableDictionaryRef */
using SFBCFArray = SFBCFWrapper<CFArrayRef>;										/*!< @brief A wrapped \c CFArrayRef */
using SFBCFMutableArray = SFBCFWrapper<CFMutableArrayRef>;							/*!< @brief A wrapped \c CFMutableArrayRef */
using SFBCFSet = SFBCFWrapper<CFSetRef>;											/*!< @brief A wrapped \c CFSetRef */
using SFBCFMutableSet = SFBCFWrapper<CFMutableSetRef>;								/*!< @brief A wrapped \c CFMutableSetRef */
using SFBCFBag = SFBCFWrapper<CFBagRef>;											/*!< @brief A wrapped \c CFBagRef */
using SFBCFMutableBag = SFBCFWrapper<CFMutableBagRef>;								/*!< @brief A wrapped \c CFMutableBagRef */
using SFBCFPropertyList = SFBCFWrapper<CFPropertyListRef>;							/*!< @brief A wrapped \c CFPropertyListRef */
using SFBCFBitVector = SFBCFWrapper<CFBitVectorRef>;								/*!< @brief A wrapped \c CFBitVectorRef */
using SFBCFMutableBitVector = SFBCFWrapper<CFMutableBitVectorRef>;					/*!< @brief A wrapped \c CFMutableBitVectorRef */
using SFBCFCharacterSet = SFBCFWrapper<CFCharacterSetRef>;							/*!< @brief A wrapped \c CFCharacterSetRef */
using SFBCFMutableCharacterSet = SFBCFWrapper<CFMutableCharacterSetRef>;			/*!< @brief A wrapped \c CFMutableCharacterSetRef */
using SFBCFURL = SFBCFWrapper<CFURLRef>;											/*!< @brief A wrapped \c CFURLRef */
using SFBCFUUID = SFBCFWrapper<CFUUIDRef>;											/*!< @brief A wrapped \c CFUUIDRef */
using SFBCFNumber = SFBCFWrapper<CFNumberRef>;										/*!< @brief A wrapped \c CFNumberRef */
using SFBCFBoolean = SFBCFWrapper<CFBooleanRef>;									/*!< @brief A wrapped \c CFBooleanRef */
using SFBCFError = SFBCFWrapper<CFErrorRef>;										/*!< @brief A wrapped \c CFErrorRef */
using SFBCFDate = SFBCFWrapper<CFDateRef>;											/*!< @brief A wrapped \c CFDateRef */
using SFBCFReadStream = SFBCFWrapper<CFReadStreamRef>;								/*!< @brief A wrapped \c CFReadStream */
using SFBCFWriteStream = SFBCFWrapper<CFWriteStreamRef>;							/*!< @brief A wrapped \c CFWriteStream */
using SFBCFHTTPMessage = SFBCFWrapper<CFHTTPMessageRef>;							/*!< @brief A wrapped \c CFHTTPMessageRef */
#if !TARGET_OS_IPHONE
using SFBSecKeychainItem = SFBCFWrapper<SecKeychainItemRef>;						/*!< @brief A wrapped \c SecKeychainItemRef */
using SFBSecCertificate = SFBCFWrapper<SecCertificateRef>;							/*!< @brief A wrapped \c SecCertificateRef */
using SFBSecTransform = SFBCFWrapper<SecTransformRef>;								/*!< @brief A wrapped \c SecTransformRef */
using SFBCGImageSource = SFBCFWrapper<CGImageSourceRef>;							/*!< @brief A wrapped \c CGImageSourceRef */
#endif

