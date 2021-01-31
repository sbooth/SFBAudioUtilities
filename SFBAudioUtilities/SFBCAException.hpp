//
// Copyright (c) 2021 Stephen F. Booth <me@sbooth.org>
// MIT license
//

#pragma once

#import <cctype>
#import <cstring>
#import <exception>
#import <system_error>

#import <CoreAudio/CoreAudio.h>
#import <AudioToolbox/AudioToolbox.h>

/// CoreAudio error codes
enum class SFBCoreAudioErrorCode
{
	noError = 0,

	// CoreAudioBaseTypes.h
	unimplementedError 		= kAudio_UnimplementedError,
	fileNotFoundError 		= kAudio_FileNotFoundError,
	filePermissionError 	= kAudio_FilePermissionError,
	tooManyFilesOpenError 	= kAudio_TooManyFilesOpenError,
	badFilePathError 		= kAudio_BadFilePathError,
	paramError 				= kAudio_ParamError,
	memFullError 			= kAudio_MemFullError,

	// AudioHardwareBase.h
//	hardwareNoError 					= kAudioHardwareNoError,
	hardwareNotRunningError 			= kAudioHardwareNotRunningError,
	hardwareUnspecifiedError 			= kAudioHardwareUnspecifiedError,
	hardwareUnknownPropertyError 		= kAudioHardwareUnknownPropertyError,
	hardwareBadPropertySizeError 		= kAudioHardwareBadPropertySizeError,
	hardwareIllegalOperationError 		= kAudioHardwareIllegalOperationError,
	hardwareBadObjectError 				= kAudioHardwareBadObjectError,
	hardwareBadDeviceError 				= kAudioHardwareBadDeviceError,
	hardwareBadStreamError 				= kAudioHardwareBadStreamError,
	hardwareUnsupportedOperationError 	= kAudioHardwareUnsupportedOperationError,
	deviceUnsupportedFormatError 		= kAudioDeviceUnsupportedFormatError,
	devicePermissionsError 				= kAudioDevicePermissionsError,
};

/// AudioUnit error codes
enum class SFBAudioUnitErrorCode
{
	noError = 0,

	// AUComponent.h
	invalidProperty 				= kAudioUnitErr_InvalidProperty,
	invalidParameter 				= kAudioUnitErr_InvalidParameter,
	invalidElement 					= kAudioUnitErr_InvalidElement,
	noConnection 					= kAudioUnitErr_NoConnection,
	failedInitialization 			= kAudioUnitErr_FailedInitialization,
	tooManyFramesToProcess 			= kAudioUnitErr_TooManyFramesToProcess,
	invalidFile 					= kAudioUnitErr_InvalidFile,
	unknownFileType 				= kAudioUnitErr_UnknownFileType,
	fileNotSpecified 				= kAudioUnitErr_FileNotSpecified,
	formatNotSupported 				= kAudioUnitErr_FormatNotSupported,
	uninitialized 					= kAudioUnitErr_Uninitialized,
	invalidScope 					= kAudioUnitErr_InvalidScope,
	propertyNotWritable 			= kAudioUnitErr_PropertyNotWritable,
	cannotDoInCurrentContext	 	= kAudioUnitErr_CannotDoInCurrentContext,
	invalidPropertyValue 			= kAudioUnitErr_InvalidPropertyValue,
	propertyNotInUse 				= kAudioUnitErr_PropertyNotInUse,
	initialized 					= kAudioUnitErr_Initialized,
	invalidOfflineRender 			= kAudioUnitErr_InvalidOfflineRender,
	unauthorized 					= kAudioUnitErr_Unauthorized,
	midiOutputBufferFull 			= kAudioUnitErr_MIDIOutputBufferFull,
	componentInstanceTimedOut 		= kAudioComponentErr_InstanceTimedOut,
	componentInstanceInvalidated 	= kAudioComponentErr_InstanceInvalidated,
	renderTimeout 					= kAudioUnitErr_RenderTimeout,
	extensionNotFound 				= kAudioUnitErr_ExtensionNotFound,
	invalidParameterValue 			= kAudioUnitErr_InvalidParameterValue,
	invalidFilePath 				= kAudioUnitErr_InvalidFilePath,
	missingKey 						= kAudioUnitErr_MissingKey,

	componentDuplicateDescription 		= kAudioComponentErr_DuplicateDescription,
	componentUnsupportedType 			= kAudioComponentErr_UnsupportedType,
	componentTooManyInstances 			= kAudioComponentErr_TooManyInstances,
	componentNotPermitted 				= kAudioComponentErr_NotPermitted,
	componentInitializationTimedOut 	= kAudioComponentErr_InitializationTimedOut,
	componentInvalidFormat 				= kAudioComponentErr_InvalidFormat,
};

// AudioCodec error codes
enum class SFBAudioCodecErrorCode
{
	noError = 0,

	// AudioCodec.h
	unspecifiedError 			= kAudioCodecUnspecifiedError,
	unknownPropertyError 		= kAudioCodecUnknownPropertyError,
	badPropertySizeError 		= kAudioCodecBadPropertySizeError,
	illegalOperationError 		= kAudioCodecIllegalOperationError,
	unsupportedFormatError 		= kAudioCodecUnsupportedFormatError,
	stateError 					= kAudioCodecStateError,
	notEnoughBufferSpaceError 	= kAudioCodecNotEnoughBufferSpaceError,
	badDataError 				= kAudioCodecBadDataError,
};

/// AudioConverter error codes
enum class SFBAudioConverterErrorCode
{
	noError = 0,

	// AudioConverter.h
	formatNotSupported 					= kAudioConverterErr_FormatNotSupported,
	operationNotSupported 				= kAudioConverterErr_OperationNotSupported,
	propertyNotSupported 				= kAudioConverterErr_PropertyNotSupported,
	invalidInputSize 					= kAudioConverterErr_InvalidInputSize,
	invalidOutputSize 					= kAudioConverterErr_InvalidOutputSize,
	unspecifiedError 					= kAudioConverterErr_UnspecifiedError,
	badPropertySizeError 				= kAudioConverterErr_BadPropertySizeError,
	requiresPacketDescriptionsError 	= kAudioConverterErr_RequiresPacketDescriptionsError,
	inputSampleRateOutOfRange 			= kAudioConverterErr_InputSampleRateOutOfRange,
	outputSampleRateOutOfRange 			= kAudioConverterErr_OutputSampleRateOutOfRange,
#if TARGET_OS_IPHONE
	hardwareInUse 						= kAudioConverterErr_HardwareInUse,
	noHardwarePermission 				= kAudioConverterErr_NoHardwarePermission,
#endif
};

/// AudioFile error codes
enum class SFBAudioFileErrorCode
{
	noError = 0,

	// AudioFile.h
	unspecifiedError 				= kAudioFileUnspecifiedError,
	unsupportedFileTypeError 		= kAudioFileUnsupportedFileTypeError,
	unsupportedDataFormatError 		= kAudioFileUnsupportedDataFormatError,
	unsupportedPropertyError 		= kAudioFileUnsupportedPropertyError,
	badPropertySizeError 			= kAudioFileBadPropertySizeError,
	permissionsError 				= kAudioFilePermissionsError,
	notOptimizedError 				= kAudioFileNotOptimizedError,

	invalidChunkError 				= kAudioFileInvalidChunkError,
	doesNotAllow64BitDataSizeError 	= kAudioFileDoesNotAllow64BitDataSizeError,
	invalidPacketOffsetError 		= kAudioFileInvalidPacketOffsetError,
	invalidPacketDependencyError 	= kAudioFileInvalidPacketDependencyError,
	invalidFileError 				= kAudioFileInvalidFileError,
	operationNotSupportedError 		= kAudioFileOperationNotSupportedError,

	notOpenError 					= kAudioFileNotOpenError,
	endOfFileError 					= kAudioFileEndOfFileError,
	positionError 					= kAudioFilePositionError,
	fileNotFoundError 				= kAudioFileFileNotFoundError,
};

/// ExtAudioFile error codes
enum class SFBExtAudioFileErrorCode
{
	noError = 0,

	// ExtAudioFile.h
	invalidProperty 					= kExtAudioFileError_InvalidProperty,
	invalidPropertySize 				= kExtAudioFileError_InvalidPropertySize,
	nonPCMClientFormat 					= kExtAudioFileError_NonPCMClientFormat,
	invalidChannelMap 					= kExtAudioFileError_InvalidChannelMap,
	invalidOperationOrder 				= kExtAudioFileError_InvalidOperationOrder,
	invalidDataFormat 					= kExtAudioFileError_InvalidDataFormat,
	maxPacketSizeUnknown 				= kExtAudioFileError_MaxPacketSizeUnknown,
	invalidSeek 						= kExtAudioFileError_InvalidSeek,
	asyncWriteTooLarge 					= kExtAudioFileError_AsyncWriteTooLarge,
	asyncWriteBufferOverflow 			= kExtAudioFileError_AsyncWriteBufferOverflow,
#if TARGET_OS_IPHONE
	codecUnavailableInputConsumed 		= kExtAudioFileError_CodecUnavailableInputConsumed,
	codecUnavailableInputNotConsumed 	= kExtAudioFileError_CodecUnavailableInputNotConsumed,
#endif

};

namespace std
{
	template <> struct is_error_code_enum<SFBCoreAudioErrorCode> : true_type
	{};

	template <> struct is_error_code_enum<SFBAudioUnitErrorCode> : true_type
	{};

	template <> struct is_error_code_enum<SFBAudioCodecErrorCode> : true_type
	{};

	template <> struct is_error_code_enum<SFBAudioConverterErrorCode> : true_type
	{};

	template <> struct is_error_code_enum<SFBAudioFileErrorCode> : true_type
	{};

	template <> struct is_error_code_enum<SFBExtAudioFileErrorCode> : true_type
	{};
}

namespace detail
{
	class SFBCoreAudioErrorCategory : public std::error_category
	{

	public:

		virtual const char * name() const noexcept override final
		{
			return "CoreAudio";
		}

		virtual std::string message(int condition) const override final
		{
			switch (static_cast<SFBCoreAudioErrorCode>(condition)) {
				case SFBCoreAudioErrorCode::noError:
					return "noError";

				case SFBCoreAudioErrorCode::unimplementedError: 	return "kAudio_UnimplementedError";
				case SFBCoreAudioErrorCode::fileNotFoundError: 		return "kAudio_FileNotFoundError";
				case SFBCoreAudioErrorCode::filePermissionError: 	return "kAudio_FilePermissionError";
				case SFBCoreAudioErrorCode::tooManyFilesOpenError: 	return "kAudio_TooManyFilesOpenError";
				case SFBCoreAudioErrorCode::badFilePathError: 		return "kAudio_BadFilePathError";
				case SFBCoreAudioErrorCode::paramError: 			return "kAudio_ParamError";
				case SFBCoreAudioErrorCode::memFullError: 			return "kAudio_MemFullError";

				case SFBCoreAudioErrorCode::hardwareNotRunningError:			return "kAudioHardwareNotRunningError";
				case SFBCoreAudioErrorCode::hardwareUnspecifiedError:			return "kAudioHardwareUnspecifiedError";
				case SFBCoreAudioErrorCode::hardwareUnknownPropertyError:		return "kAudioHardwareUnknownPropertyError";
				case SFBCoreAudioErrorCode::hardwareBadPropertySizeError:		return "kAudioHardwareBadPropertySizeError";
				case SFBCoreAudioErrorCode::hardwareIllegalOperationError:		return "kAudioHardwareIllegalOperationError";
				case SFBCoreAudioErrorCode::hardwareBadObjectError:				return "kAudioHardwareBadObjectError";
				case SFBCoreAudioErrorCode::hardwareBadDeviceError:				return "kAudioHardwareBadDeviceError";
				case SFBCoreAudioErrorCode::hardwareBadStreamError:				return "kAudioHardwareBadStreamError";
				case SFBCoreAudioErrorCode::hardwareUnsupportedOperationError: 	return "kAudioHardwareUnsupportedOperationError";
				case SFBCoreAudioErrorCode::deviceUnsupportedFormatError:		return "kAudioDeviceUnsupportedFormatError";
				case SFBCoreAudioErrorCode::devicePermissionsError:				return "kAudioDevicePermissionsError";

				default:
					return "unknown";
			}
		}
	};

	class SFBAudioUnitErrorCategory : public std::error_category
	{

	public:

		virtual const char * name() const noexcept override final
		{
			return "AudioUnit";
		}

		virtual std::string message(int condition) const override final
		{
			switch (static_cast<SFBAudioUnitErrorCode>(condition)) {
				case SFBAudioUnitErrorCode::noError:
					return "noError";

				case SFBAudioUnitErrorCode::invalidProperty: 				return "kAudioUnitErr_InvalidProperty";
				case SFBAudioUnitErrorCode::invalidParameter: 				return "kAudioUnitErr_InvalidParameter";
				case SFBAudioUnitErrorCode::invalidElement: 				return "kAudioUnitErr_InvalidElement";
				case SFBAudioUnitErrorCode::noConnection: 					return "kAudioUnitErr_NoConnection";
				case SFBAudioUnitErrorCode::failedInitialization: 			return "kAudioUnitErr_FailedInitialization";
				case SFBAudioUnitErrorCode::tooManyFramesToProcess: 		return "kAudioUnitErr_TooManyFramesToProcess";
				case SFBAudioUnitErrorCode::invalidFile: 					return "kAudioUnitErr_InvalidFile";
				case SFBAudioUnitErrorCode::unknownFileType: 				return "kAudioUnitErr_UnknownFileType";
				case SFBAudioUnitErrorCode::fileNotSpecified: 				return "kAudioUnitErr_FileNotSpecified";
				case SFBAudioUnitErrorCode::formatNotSupported: 			return "kAudioUnitErr_FormatNotSupported";
				case SFBAudioUnitErrorCode::uninitialized: 					return "kAudioUnitErr_Uninitialized";
				case SFBAudioUnitErrorCode::invalidScope: 					return "kAudioUnitErr_InvalidScope";
				case SFBAudioUnitErrorCode::propertyNotWritable: 			return "kAudioUnitErr_PropertyNotWritable";
				case SFBAudioUnitErrorCode::cannotDoInCurrentContext: 		return "kAudioUnitErr_CannotDoInCurrentContext";
				case SFBAudioUnitErrorCode::invalidPropertyValue: 			return "kAudioUnitErr_InvalidPropertyValue";
				case SFBAudioUnitErrorCode::propertyNotInUse: 				return "kAudioUnitErr_PropertyNotInUse";
				case SFBAudioUnitErrorCode::initialized: 					return "kAudioUnitErr_Initialized";
				case SFBAudioUnitErrorCode::invalidOfflineRender: 			return "kAudioUnitErr_InvalidOfflineRender";
				case SFBAudioUnitErrorCode::unauthorized: 					return "kAudioUnitErr_Unauthorized";
				case SFBAudioUnitErrorCode::midiOutputBufferFull: 			return "kAudioUnitErr_MIDIOutputBufferFull";
				case SFBAudioUnitErrorCode::componentInstanceTimedOut: 		return "kAudioComponentErr_InstanceTimedOut";
				case SFBAudioUnitErrorCode::componentInstanceInvalidated: 	return "kAudioComponentErr_InstanceInvalidated";
				case SFBAudioUnitErrorCode::renderTimeout: 					return "kAudioUnitErr_RenderTimeout";
				case SFBAudioUnitErrorCode::extensionNotFound: 				return "kAudioUnitErr_ExtensionNotFound";
				case SFBAudioUnitErrorCode::invalidParameterValue: 			return "kAudioUnitErr_InvalidParameterValue";
				case SFBAudioUnitErrorCode::invalidFilePath: 				return "kAudioUnitErr_InvalidFilePath";
				case SFBAudioUnitErrorCode::missingKey: 					return "kAudioUnitErr_MissingKey";

				case SFBAudioUnitErrorCode::componentDuplicateDescription: 		return "kAudioComponentErr_DuplicateDescription";
				case SFBAudioUnitErrorCode::componentUnsupportedType: 			return "kAudioComponentErr_UnsupportedType";
				case SFBAudioUnitErrorCode::componentTooManyInstances: 			return "kAudioComponentErr_TooManyInstances";
				case SFBAudioUnitErrorCode::componentNotPermitted: 				return "kAudioComponentErr_NotPermitted";
				case SFBAudioUnitErrorCode::componentInitializationTimedOut: 	return "kAudioComponentErr_InitializationTimedOut";
				case SFBAudioUnitErrorCode::componentInvalidFormat: 			return "kAudioComponentErr_InvalidFormat";

				default:
					return "unknown";
			}
		}
	};

	class SFBAudioCodecErrorCategory : public std::error_category
	{

	public:

		virtual const char * name() const noexcept override final
		{
			return "AudioCodec";
		}

		virtual std::string message(int condition) const override final
		{
			switch (static_cast<SFBAudioCodecErrorCode>(condition)) {
				case SFBAudioCodecErrorCode::noError:
					return "noError";

				case SFBAudioCodecErrorCode::unspecifiedError: 				return "kAudioCodecUnspecifiedError";
				case SFBAudioCodecErrorCode::unknownPropertyError: 			return "kAudioCodecUnknownPropertyError";
				case SFBAudioCodecErrorCode::badPropertySizeError: 			return "kAudioCodecBadPropertySizeError";
				case SFBAudioCodecErrorCode::illegalOperationError: 		return "kAudioCodecIllegalOperationError";
				case SFBAudioCodecErrorCode::unsupportedFormatError: 		return "kAudioCodecUnsupportedFormatError";
				case SFBAudioCodecErrorCode::stateError: 					return "kAudioCodecStateError";
				case SFBAudioCodecErrorCode::notEnoughBufferSpaceError: 	return "kAudioCodecNotEnoughBufferSpaceError";
				case SFBAudioCodecErrorCode::badDataError: 					return "kAudioCodecBadDataError";

				default:
					return "unknown";
			}
		}
	};
	class SFBAudioConverterErrorCategory : public std::error_category
	{

	public:

		virtual const char * name() const noexcept override final
		{
			return "AudioConverter";
		}

		virtual std::string message(int condition) const override final
		{
			switch (static_cast<SFBAudioConverterErrorCode>(condition)) {
				case SFBAudioConverterErrorCode::noError:
					return "noError";

				case SFBAudioConverterErrorCode::formatNotSupported: 				return "kAudioConverterErr_FormatNotSupported or kAudioFileUnsupportedDataFormatError";
				case SFBAudioConverterErrorCode::operationNotSupported: 			return "kAudioConverterErr_OperationNotSupported";
				case SFBAudioConverterErrorCode::propertyNotSupported: 				return "kAudioConverterErr_PropertyNotSupported";
				case SFBAudioConverterErrorCode::invalidInputSize: 					return "kAudioConverterErr_InvalidInputSize";
				case SFBAudioConverterErrorCode::invalidOutputSize: 				return "kAudioConverterErr_InvalidOutputSize";
				case SFBAudioConverterErrorCode::unspecifiedError: 					return "kAudioConverterErr_UnspecifiedError";
				case SFBAudioConverterErrorCode::badPropertySizeError: 				return "kAudioConverterErr_BadPropertySizeError";
				case SFBAudioConverterErrorCode::requiresPacketDescriptionsError: 	return "kAudioConverterErr_RequiresPacketDescriptionsError";
				case SFBAudioConverterErrorCode::inputSampleRateOutOfRange: 		return "kAudioConverterErr_InputSampleRateOutOfRange";
				case SFBAudioConverterErrorCode::outputSampleRateOutOfRange: 		return "kAudioConverterErr_OutputSampleRateOutOfRange";
#if TARGET_OS_IPHONE
				case SFBAudioConverterErrorCode::hardwareInUse: 					return "kAudioConverterErr_HardwareInUse";
				case SFBAudioConverterErrorCode::noHardwarePermission: 				return "kAudioConverterErr_NoHardwarePermission";
#endif

				default:
					return "unknown";
			}
		}
	};

	class SFBAudioFileErrorCategory : public std::error_category
	{

	public:

		virtual const char * name() const noexcept override final
		{
			return "AudioFile";
		}

		virtual std::string message(int condition) const override final
		{
			switch (static_cast<SFBAudioFileErrorCode>(condition)) {
				case SFBAudioFileErrorCode::noError:
					return "noError";

				case SFBAudioFileErrorCode::unspecifiedError: 					return "kAudioFileUnspecifiedError";
				case SFBAudioFileErrorCode::unsupportedFileTypeError: 			return "kAudioFileUnsupportedFileTypeError";
				case SFBAudioFileErrorCode::unsupportedDataFormatError: 		return "kAudioFileUnsupportedDataFormatError";
				case SFBAudioFileErrorCode::unsupportedPropertyError: 			return "kAudioFileUnsupportedPropertyError";
				case SFBAudioFileErrorCode::badPropertySizeError: 				return "kAudioFileBadPropertySizeError";
				case SFBAudioFileErrorCode::permissionsError: 					return "kAudioFilePermissionsError";
				case SFBAudioFileErrorCode::notOptimizedError: 					return "kAudioFileNotOptimizedError";

				case SFBAudioFileErrorCode::invalidChunkError: 					return "kAudioFileInvalidChunkError";
				case SFBAudioFileErrorCode::doesNotAllow64BitDataSizeError: 	return "kAudioFileDoesNotAllow64BitDataSizeError";
				case SFBAudioFileErrorCode::invalidPacketOffsetError: 			return "kAudioFileInvalidPacketOffsetError";
				case SFBAudioFileErrorCode::invalidPacketDependencyError: 		return "kAudioFileInvalidPacketDependencyError";
				case SFBAudioFileErrorCode::invalidFileError:					return "kAudioFileInvalidFileError";
				case SFBAudioFileErrorCode::operationNotSupportedError: 		return "kAudioFileOperationNotSupportedError";

				case SFBAudioFileErrorCode::notOpenError:						return "kAudioFileNotOpenError";
				case SFBAudioFileErrorCode::endOfFileError: 					return "kAudioFileEndOfFileError";
				case SFBAudioFileErrorCode::positionError: 						return "kAudioFilePositionError";
				case SFBAudioFileErrorCode::fileNotFoundError: 					return "kAudioFileFileNotFoundError";

				default:
					return "unknown";
			}
		}
	};


	class SFBExtAudioFileErrorCategory : public std::error_category
	{

	public:

		virtual const char * name() const noexcept override final
		{
			return "ExtAudioFile";
		}

		virtual std::string message(int condition) const override final
		{
			switch (static_cast<SFBExtAudioFileErrorCode>(condition)) {
				case SFBExtAudioFileErrorCode::noError:
					return "noError";

				case SFBExtAudioFileErrorCode::invalidProperty:						return "kExtAudioFileError_InvalidProperty";
				case SFBExtAudioFileErrorCode::invalidPropertySize: 				return "kExtAudioFileError_InvalidPropertySize";
				case SFBExtAudioFileErrorCode::nonPCMClientFormat: 					return "kExtAudioFileError_NonPCMClientFormat";
				case SFBExtAudioFileErrorCode::invalidChannelMap: 					return "kExtAudioFileError_InvalidChannelMap";
				case SFBExtAudioFileErrorCode::invalidOperationOrder: 				return "kExtAudioFileError_InvalidOperationOrder";
				case SFBExtAudioFileErrorCode::invalidDataFormat: 					return "kExtAudioFileError_InvalidDataFormat";
				case SFBExtAudioFileErrorCode::maxPacketSizeUnknown: 				return "kExtAudioFileError_MaxPacketSizeUnknown";
				case SFBExtAudioFileErrorCode::invalidSeek: 						return "kExtAudioFileError_InvalidSeek";
				case SFBExtAudioFileErrorCode::asyncWriteTooLarge: 					return "kExtAudioFileError_AsyncWriteTooLarge";
				case SFBExtAudioFileErrorCode::asyncWriteBufferOverflow: 			return "kExtAudioFileError_AsyncWriteBufferOverflow";
#if TARGET_OS_IPHONE
				case SFBExtAudioFileErrorCode::codecUnavailableInputConsumed:		return "kExtAudioFileError_CodecUnavailableInputConsumed";
				case SFBExtAudioFileErrorCode::codecUnavailableInputNotConsumed: 	return "kExtAudioFileError_CodecUnavailableInputNotConsumed";
#endif

				default:
					return "unknown";
			}
		}
	};
}

extern inline const detail::SFBCoreAudioErrorCategory& SFBCoreAudioErrorCategory()
{
	static detail::SFBCoreAudioErrorCategory c;
	return c;
}

extern inline const detail::SFBAudioUnitErrorCategory& SFBAudioUnitErrorCategory()
{
	static detail::SFBAudioUnitErrorCategory c;
	return c;
}

extern inline const detail::SFBAudioCodecErrorCategory& SFBAudioCodecErrorCategory()
{
	static detail::SFBAudioCodecErrorCategory c;
	return c;
}

extern inline const detail::SFBAudioConverterErrorCategory& SFBAudioConverterErrorCategory()
{
	static detail::SFBAudioConverterErrorCategory c;
	return c;
}

extern inline const detail::SFBAudioFileErrorCategory& SFBAudioFileErrorCategory()
{
	static detail::SFBAudioFileErrorCategory c;
	return c;
}

extern inline const detail::SFBExtAudioFileErrorCategory& SFBExtAudioFileErrorCategory()
{
	static detail::SFBExtAudioFileErrorCategory c;
	return c;
}

inline std::error_code make_error_code(SFBCoreAudioErrorCode e)
{
	return { static_cast<int>(e), SFBCoreAudioErrorCategory() };
}

inline std::error_code make_error_code(SFBAudioUnitErrorCode e)
{
	return { static_cast<int>(e), SFBAudioUnitErrorCategory() };
}

inline std::error_code make_error_code(SFBAudioCodecErrorCode e)
{
	return { static_cast<int>(e), SFBAudioCodecErrorCategory() };
}

inline std::error_code make_error_code(SFBAudioConverterErrorCode e)
{
	return { static_cast<int>(e), SFBAudioConverterErrorCategory() };
}

inline std::error_code make_error_code(SFBAudioFileErrorCode e)
{
	return { static_cast<int>(e), SFBAudioFileErrorCategory() };
}

inline std::error_code make_error_code(SFBExtAudioFileErrorCode e)
{
	return { static_cast<int>(e), SFBExtAudioFileErrorCategory() };
}

/// Throws a @c std::system_error in the @c SFBCoreAudioErrorCategory if @c result!=0
/// @param result An @c OSStatus result code
/// @param operation An optional string describing the operation producing @c result
/// @throw @c std::system_error in the @c SFBCoreAudioErrorCategory
inline void SFBThrowIfCoreAudioError(OSStatus result, const char * const operation = nullptr)
{
	if(__builtin_expect(result != 0, 0))
		throw std::system_error(result, SFBCoreAudioErrorCategory(), operation);
}

/// Throws a @c std::system_error in the @c SFBAudioUnitErrorCategory if @c result!=0
/// @param result An @c OSStatus result code
/// @param operation An optional string describing the operation producing @c result
/// @throw @c std::system_error in the @c SFBAudioUnitErrorCategory
inline void SFBThrowIfAudioUnitError(OSStatus result, const char * const operation = nullptr)
{
	if(__builtin_expect(result != 0, 0))
		throw std::system_error(result, SFBAudioUnitErrorCategory(), operation);
}

/// Throws a @c std::system_error in the @c SFBAudioCodecErrorCategory if @c result!=0
/// @param result An @c OSStatus result code
/// @param operation An optional string describing the operation producing @c result
/// @throw @c std::system_error in the @c SFBAudioCodecErrorCategory
inline void SFBThrowIfAudioCodecError(OSStatus result, const char * const operation = nullptr)
{
	if(__builtin_expect(result != 0, 0))
		throw std::system_error(result, SFBAudioCodecErrorCategory(), operation);
}

/// Throws a @c std::system_error in the @c SFBAudioConverterErrorCategory if @c result!=0
/// @param result An @c OSStatus result code
/// @param operation An optional string describing the operation producing @c result
/// @throw @c std::system_error in the @c SFBAudioConverterErrorCategory
inline void SFBThrowIfAudioConverterError(OSStatus result, const char * const operation = nullptr)
{
	if(__builtin_expect(result != 0, 0))
		throw std::system_error(result, SFBAudioConverterErrorCategory(), operation);
}

/// Throws a @c std::system_error in the @c SFBAudioFileErrorCategory if @c result!=0
/// @param result An @c OSStatus result code
/// @param operation An optional string describing the operation producing @c result
/// @throw @c std::system_error in the @c SFBAudioFileErrorCategory
inline void SFBThrowIfAudioFileError(OSStatus result, const char * const operation = nullptr)
{
	if(__builtin_expect(result != 0, 0))
		throw std::system_error(result, SFBAudioFileErrorCategory(), operation);
}

/// Throws a @c std::system_error in the @c SFBExtAudioFileErrorCategory if @c result!=0
/// @param result An @c OSStatus result code
/// @param operation An optional string describing the operation producing @c result
/// @throw @c std::system_error in the @c SFBExtAudioFileErrorCategory
inline void SFBThrowIfExtAudioFileError(OSStatus result, const char * const operation = nullptr)
{
	if(__builtin_expect(result != 0, 0))
		throw std::system_error(result, SFBExtAudioFileErrorCategory(), operation);
}

#if 0
class SFBFourCCString
{

public:

	SFBFourCCString(UInt32 errorCode)
	{
		auto err = CFSwapInt32HostToBig(errorCode);
		std::memcpy(&mString[0] + 1, &err, 4);
		if(std::isprint(mString[1]) && std::isprint(mString[2]) && std::isprint(mString[3]) && std::isprint(mString[4])) {
			mString[0] = mString[5] = '\'';
			mString[6] = '\0';
		}
		else if(errorCode > -200000 && errorCode < 200000)
			snprintf(mString, sizeof(mString), "%d", static_cast<int>(errorCode));
		else
			snprintf(mString, sizeof(mString), "0x%x", static_cast<int>(errorCode));
	}

	operator const char * const () const
	{
		return mString;
	}

private:

	char mString [16];

};
#endif
