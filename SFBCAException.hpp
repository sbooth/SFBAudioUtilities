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

/// CoreAudio and related framework common error codes
enum class SFBGeneralAudioErrorCode {
	noError = 0,

	// CoreAudioBaseTypes.h
	unimplementedError 		= kAudio_UnimplementedError,
	fileNotFoundError 		= kAudio_FileNotFoundError,
	filePermissionError 	= kAudio_FilePermissionError,
	tooManyFilesOpenError 	= kAudio_TooManyFilesOpenError,
	badFilePathError 		= kAudio_BadFilePathError,
	paramError 				= kAudio_ParamError,
	memFullError 			= kAudio_MemFullError,
};

/// AudioObject error codes
enum class SFBAudioObjectErrorCode {
	noError = 0,

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
enum class SFBAudioUnitErrorCode {
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
enum class SFBAudioCodecErrorCode {
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
enum class SFBAudioConverterErrorCode {
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
enum class SFBAudioFileErrorCode {
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
enum class SFBExtAudioFileErrorCode {
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
	template <> struct is_error_code_enum<SFBAudioObjectErrorCode> : true_type
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

	class SFBAudioObjectErrorCategory : public std::error_category
	{

	public:

		virtual const char * name() const noexcept override final
		{
			return "AudioObject";
		}

		virtual std::string message(int condition) const override final
		{
			switch(static_cast<SFBAudioObjectErrorCode>(condition)) {
				case SFBAudioObjectErrorCode::hardwareNotRunningError:				return "The function call requires that the hardware be running but it isn't";
				case SFBAudioObjectErrorCode::hardwareUnspecifiedError:				return "The function call failed while doing something that doesn't provide any error messages";
				case SFBAudioObjectErrorCode::hardwareUnknownPropertyError:			return "The AudioObject doesn't know about the property at the given address";
				case SFBAudioObjectErrorCode::hardwareBadPropertySizeError:			return "An improperly sized buffer was provided when accessing the data of a property";
				case SFBAudioObjectErrorCode::hardwareIllegalOperationError:		return "The requested operation couldn't be completed";
				case SFBAudioObjectErrorCode::hardwareBadObjectError:				return "The AudioObjectID passed to the function doesn't map to a valid AudioObject";
				case SFBAudioObjectErrorCode::hardwareBadDeviceError:				return "The AudioObjectID passed to the function doesn't map to a valid AudioDevice";
				case SFBAudioObjectErrorCode::hardwareBadStreamError:				return "The AudioObjectID passed to the function doesn't map to a valid AudioStream";
				case SFBAudioObjectErrorCode::hardwareUnsupportedOperationError: 	return "The AudioObject doesn't support the requested operation";
				case SFBAudioObjectErrorCode::deviceUnsupportedFormatError:			return "The AudioStream doesn't support the requested format";
				case SFBAudioObjectErrorCode::devicePermissionsError:				return "The requested operation can't be completed because the process doesn't have permission";

				default:
					switch(static_cast<SFBGeneralAudioErrorCode>(condition)) {
						case SFBGeneralAudioErrorCode::noError: 				return "The function call completed successfully";

						case SFBGeneralAudioErrorCode::unimplementedError: 		return "Unimplemented core routine";
						case SFBGeneralAudioErrorCode::fileNotFoundError: 		return "File not found";
						case SFBGeneralAudioErrorCode::filePermissionError: 	return "File cannot be opened due to either file, directory, or sandbox permissions";
						case SFBGeneralAudioErrorCode::tooManyFilesOpenError: 	return "File cannot be opened because too many files are already open";
						case SFBGeneralAudioErrorCode::badFilePathError: 		return "File cannot be opened because the specified path is malformed";
						case SFBGeneralAudioErrorCode::paramError: 				return "Error in user parameter list";
						case SFBGeneralAudioErrorCode::memFullError: 			return "Not enough room in heap zone";

						default:												return "unknown";
					}
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
			switch(static_cast<SFBAudioUnitErrorCode>(condition)) {
				case SFBAudioUnitErrorCode::invalidProperty: 				return "The property is not supported";
				case SFBAudioUnitErrorCode::invalidParameter: 				return "The parameter is not supported";
				case SFBAudioUnitErrorCode::invalidElement: 				return "The specified element is not valid";
				case SFBAudioUnitErrorCode::noConnection: 					return "There is no connection (generally an audio unit is asked to render but it has not input from which to gather data)";
				case SFBAudioUnitErrorCode::failedInitialization: 			return "The audio unit is unable to be initialized";
				case SFBAudioUnitErrorCode::tooManyFramesToProcess: 		return "When an audio unit is initialized it has a value which specifies the max number of frames it will be asked to render at any given time. If an audio unit is asked to render more than this, this error is returned";
				case SFBAudioUnitErrorCode::invalidFile: 					return "If an audio unit uses external files as a data source, this error is returned if a file is invalid (Apple's DLS synth returns this error)";
				case SFBAudioUnitErrorCode::unknownFileType: 				return "If an audio unit uses external files as a data source, this error is returned if a file is invalid (Apple's DLS synth returns this error)";
				case SFBAudioUnitErrorCode::fileNotSpecified: 				return "If an audio unit uses external files as a data source, this error is returned if a file hasn't been set on it (Apple's DLS synth returns this error)";
				case SFBAudioUnitErrorCode::formatNotSupported: 			return "Returned if an input or output format is not supported";
				case SFBAudioUnitErrorCode::uninitialized: 					return "Returned if an operation requires an audio unit to be initialized and it is not";
				case SFBAudioUnitErrorCode::invalidScope: 					return "The specified scope is invalid";
				case SFBAudioUnitErrorCode::propertyNotWritable: 			return "The property cannot be written";
				case SFBAudioUnitErrorCode::cannotDoInCurrentContext: 		return "Returned when an audio unit is in a state where it can't perform the requested action now - but it could later. It's usually used to guard a render operation when a reconfiguration of its internal state is being performed";
				case SFBAudioUnitErrorCode::invalidPropertyValue: 			return "The property is valid, but the value of the property being provided is not";
				case SFBAudioUnitErrorCode::propertyNotInUse: 				return "Returned when a property is valid, but it hasn't been set to a valid value at this time";
				case SFBAudioUnitErrorCode::initialized: 					return "Indicates the operation cannot be performed because the audio unit is initialized";
				case SFBAudioUnitErrorCode::invalidOfflineRender: 			return "Used to indicate that the offline render operation is invalid. For instance, when the audio unit needs to be pre-flighted, but it hasn't been";
				case SFBAudioUnitErrorCode::unauthorized: 					return "Returned by either Open or Initialize, this error is used to indicate that the audio unit is not authorised, that it cannot be used. A host can then present a UI to notify the user the audio unit is not able to be used in its current state";
				case SFBAudioUnitErrorCode::midiOutputBufferFull: 			return "Returned during the render call, if the audio unit produces more MIDI output, than the default allocated buffer. The audio unit can provide a size hint, in case it needs a larger buffer. See the documentation for AUAudioUnit's MIDIOutputBufferSizeHint property";
				case SFBAudioUnitErrorCode::componentInstanceTimedOut: 		return "kAudioComponentErr_InstanceTimedOut";
				case SFBAudioUnitErrorCode::componentInstanceInvalidated: 	return "The component instance's implementation is not available, most likely because the process that published it is no longer running";
				case SFBAudioUnitErrorCode::renderTimeout: 					return "The audio unit did not satisfy the render request in time";
				case SFBAudioUnitErrorCode::extensionNotFound: 				return "The specified identifier did not match any Audio Unit Extensions";
				case SFBAudioUnitErrorCode::invalidParameterValue: 			return "The parameter value is not supported, e.g. the value specified is NaN or infinite";
				case SFBAudioUnitErrorCode::invalidFilePath: 				return "The file path that was passed is not supported. It is either too long or contains invalid characters";
				case SFBAudioUnitErrorCode::missingKey: 					return "A required key is missing from a dictionary object";

				case SFBAudioUnitErrorCode::componentDuplicateDescription: 		return "A non-unique component description was provided to AudioOutputUnitPublish";
				case SFBAudioUnitErrorCode::componentUnsupportedType: 			return "An unsupported component type was provided to AudioOutputUnitPublish";
				case SFBAudioUnitErrorCode::componentTooManyInstances: 			return "Components published via AudioOutputUnitPublish may only have one instance";
				case SFBAudioUnitErrorCode::componentNotPermitted: 				return "App needs \"inter-app-audio\" entitlement or host app needs \"audio\" in its UIBackgroundModes. Or app is trying to register a component not declared in its Info.plist";
				case SFBAudioUnitErrorCode::componentInitializationTimedOut: 	return "Host did not render in a timely manner; must uninitialize and reinitialize";
				case SFBAudioUnitErrorCode::componentInvalidFormat: 			return "Inter-app AU element formats must have sample rates matching the hardware";

				default:
					switch(static_cast<SFBGeneralAudioErrorCode>(condition)) {
						case SFBGeneralAudioErrorCode::noError: 				return "The function call completed successfully";

						case SFBGeneralAudioErrorCode::unimplementedError: 		return "Unimplemented core routine";
						case SFBGeneralAudioErrorCode::fileNotFoundError: 		return "File not found";
						case SFBGeneralAudioErrorCode::filePermissionError: 	return "File cannot be opened due to either file, directory, or sandbox permissions";
						case SFBGeneralAudioErrorCode::tooManyFilesOpenError: 	return "File cannot be opened because too many files are already open";
						case SFBGeneralAudioErrorCode::badFilePathError: 		return "File cannot be opened because the specified path is malformed";
						case SFBGeneralAudioErrorCode::paramError: 				return "Error in user parameter list";
						case SFBGeneralAudioErrorCode::memFullError: 			return "Not enough room in heap zone";

						default:												return "unknown";
					}
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
			switch(static_cast<SFBAudioCodecErrorCode>(condition)) {
				case SFBAudioCodecErrorCode::unspecifiedError: 				return "kAudioCodecUnspecifiedError";
				case SFBAudioCodecErrorCode::unknownPropertyError: 			return "kAudioCodecUnknownPropertyError";
				case SFBAudioCodecErrorCode::badPropertySizeError: 			return "kAudioCodecBadPropertySizeError";
				case SFBAudioCodecErrorCode::illegalOperationError: 		return "kAudioCodecIllegalOperationError";
				case SFBAudioCodecErrorCode::unsupportedFormatError: 		return "kAudioCodecUnsupportedFormatError";
				case SFBAudioCodecErrorCode::stateError: 					return "kAudioCodecStateError";
				case SFBAudioCodecErrorCode::notEnoughBufferSpaceError: 	return "kAudioCodecNotEnoughBufferSpaceError";
				case SFBAudioCodecErrorCode::badDataError: 					return "kAudioCodecBadDataError";

				default:
					switch(static_cast<SFBGeneralAudioErrorCode>(condition)) {
						case SFBGeneralAudioErrorCode::noError: 				return "The function call completed successfully";

						case SFBGeneralAudioErrorCode::unimplementedError: 		return "Unimplemented core routine";
						case SFBGeneralAudioErrorCode::fileNotFoundError: 		return "File not found";
						case SFBGeneralAudioErrorCode::filePermissionError: 	return "File cannot be opened due to either file, directory, or sandbox permissions";
						case SFBGeneralAudioErrorCode::tooManyFilesOpenError: 	return "File cannot be opened because too many files are already open";
						case SFBGeneralAudioErrorCode::badFilePathError: 		return "File cannot be opened because the specified path is malformed";
						case SFBGeneralAudioErrorCode::paramError: 				return "Error in user parameter list";
						case SFBGeneralAudioErrorCode::memFullError: 			return "Not enough room in heap zone";

						default:												return "unknown";
					}
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
			switch(static_cast<SFBAudioConverterErrorCode>(condition)) {
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
					switch(static_cast<SFBGeneralAudioErrorCode>(condition)) {
						case SFBGeneralAudioErrorCode::noError: 				return "The function call completed successfully";

						case SFBGeneralAudioErrorCode::unimplementedError: 		return "Unimplemented core routine";
						case SFBGeneralAudioErrorCode::fileNotFoundError: 		return "File not found";
						case SFBGeneralAudioErrorCode::filePermissionError: 	return "File cannot be opened due to either file, directory, or sandbox permissions";
						case SFBGeneralAudioErrorCode::tooManyFilesOpenError: 	return "File cannot be opened because too many files are already open";
						case SFBGeneralAudioErrorCode::badFilePathError: 		return "File cannot be opened because the specified path is malformed";
						case SFBGeneralAudioErrorCode::paramError: 				return "Error in user parameter list";
						case SFBGeneralAudioErrorCode::memFullError: 			return "Not enough room in heap zone";

						default:												return "unknown";
					}
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
			switch(static_cast<SFBAudioFileErrorCode>(condition)) {
				case SFBAudioFileErrorCode::unspecifiedError: 					return "An unspecified error has occurred";
				case SFBAudioFileErrorCode::unsupportedFileTypeError: 			return "The file type is not supported";
				case SFBAudioFileErrorCode::unsupportedDataFormatError: 		return "The data format is not supported by this file type";
				case SFBAudioFileErrorCode::unsupportedPropertyError: 			return "The property is not supported";
				case SFBAudioFileErrorCode::badPropertySizeError: 				return "The size of the property data was not correct";
				case SFBAudioFileErrorCode::permissionsError: 					return "The operation violated the file permissions";
				case SFBAudioFileErrorCode::notOptimizedError: 					return "There are chunks following the audio data chunk that prevent extending the audio data chunk. The file must be optimized in order to write more audio data.";

				case SFBAudioFileErrorCode::invalidChunkError: 					return "The chunk does not exist in the file or is not supported by the file";
				case SFBAudioFileErrorCode::doesNotAllow64BitDataSizeError: 	return "The a file offset was too large for the file type. AIFF and WAVE have a 32 bit file size limit.";
				case SFBAudioFileErrorCode::invalidPacketOffsetError: 			return "A packet offset was past the end of the file, or not at the end of the file when writing a VBR format, or a corrupt packet size was read when building the packet table.";
				case SFBAudioFileErrorCode::invalidPacketDependencyError: 		return "Either the packet dependency info that's necessary for the audio format has not been provided, or the provided packet dependency info indicates dependency on a packet that's unavailable.";
				case SFBAudioFileErrorCode::invalidFileError:					return "The file is malformed, or otherwise not a valid instance of an audio file of its type";
				case SFBAudioFileErrorCode::operationNotSupportedError: 		return "The operation cannot be performed";

				case SFBAudioFileErrorCode::notOpenError:						return "The file is closed";
				case SFBAudioFileErrorCode::endOfFileError: 					return "End of file";
				case SFBAudioFileErrorCode::positionError: 						return "Invalid file position";
				case SFBAudioFileErrorCode::fileNotFoundError: 					return "File not found";

				default:
					switch(static_cast<SFBGeneralAudioErrorCode>(condition)) {
						case SFBGeneralAudioErrorCode::noError: 				return "The function call completed successfully";

						case SFBGeneralAudioErrorCode::unimplementedError: 		return "Unimplemented core routine";
						case SFBGeneralAudioErrorCode::fileNotFoundError: 		return "File not found";
						case SFBGeneralAudioErrorCode::filePermissionError: 	return "File cannot be opened due to either file, directory, or sandbox permissions";
						case SFBGeneralAudioErrorCode::tooManyFilesOpenError: 	return "File cannot be opened because too many files are already open";
						case SFBGeneralAudioErrorCode::badFilePathError: 		return "File cannot be opened because the specified path is malformed";
						case SFBGeneralAudioErrorCode::paramError: 				return "Error in user parameter list";
						case SFBGeneralAudioErrorCode::memFullError: 			return "Not enough room in heap zone";

						default:												return "unknown";
					}
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
			switch(static_cast<SFBExtAudioFileErrorCode>(condition)) {
				case SFBExtAudioFileErrorCode::invalidProperty:						return "kExtAudioFileError_InvalidProperty";
				case SFBExtAudioFileErrorCode::invalidPropertySize: 				return "kExtAudioFileError_InvalidPropertySize";
				case SFBExtAudioFileErrorCode::nonPCMClientFormat: 					return "kExtAudioFileError_NonPCMClientFormat";
				case SFBExtAudioFileErrorCode::invalidChannelMap: 					return "number of channels doesn't match format";
				case SFBExtAudioFileErrorCode::invalidOperationOrder: 				return "kExtAudioFileError_InvalidOperationOrder";
				case SFBExtAudioFileErrorCode::invalidDataFormat: 					return "kExtAudioFileError_InvalidDataFormat";
				case SFBExtAudioFileErrorCode::maxPacketSizeUnknown: 				return "kExtAudioFileError_MaxPacketSizeUnknown";
				case SFBExtAudioFileErrorCode::invalidSeek: 						return "writing, or offset out of bounds";
				case SFBExtAudioFileErrorCode::asyncWriteTooLarge: 					return "kExtAudioFileError_AsyncWriteTooLarge";
				case SFBExtAudioFileErrorCode::asyncWriteBufferOverflow: 			return "an async write could not be completed in time";
#if TARGET_OS_IPHONE
				case SFBExtAudioFileErrorCode::codecUnavailableInputConsumed:		return "iOS only. Returned when ExtAudioFileWrite was interrupted. You must stop calling ExtAudioFileWrite. If the underlying audio converter can resume after an interruption (see kAudioConverterPropertyCanResumeFromInterruption), you must wait for an EndInterruption notification from AudioSession, and call AudioSessionSetActive(true) before resuming. In this situation, the buffer you provided to ExtAudioFileWrite was successfully consumed and you may proceed to the next buffer";
				case SFBExtAudioFileErrorCode::codecUnavailableInputNotConsumed: 	return "iOS only. Returned when ExtAudioFileWrite was interrupted. You must stop calling ExtAudioFileWrite. If the underlying audio converter can resume after an interruption (see kAudioConverterPropertyCanResumeFromInterruption), you must wait for an EndInterruption notification from AudioSession, and call AudioSessionSetActive(true) before resuming. In this situation, the buffer you provided to ExtAudioFileWrite was not successfully consumed and you must try to write it again";
#endif

				default:
					switch(static_cast<SFBGeneralAudioErrorCode>(condition)) {
						case SFBGeneralAudioErrorCode::noError: 				return "The function call completed successfully";

						case SFBGeneralAudioErrorCode::unimplementedError: 		return "Unimplemented core routine";
						case SFBGeneralAudioErrorCode::fileNotFoundError: 		return "File not found";
						case SFBGeneralAudioErrorCode::filePermissionError: 	return "File cannot be opened due to either file, directory, or sandbox permissions";
						case SFBGeneralAudioErrorCode::tooManyFilesOpenError: 	return "File cannot be opened because too many files are already open";
						case SFBGeneralAudioErrorCode::badFilePathError: 		return "File cannot be opened because the specified path is malformed";
						case SFBGeneralAudioErrorCode::paramError: 				return "Error in user parameter list";
						case SFBGeneralAudioErrorCode::memFullError: 			return "Not enough room in heap zone";

						default:												return "unknown";
					}
			}
		}
	};
}

extern inline const detail::SFBAudioObjectErrorCategory& SFBAudioObjectErrorCategory()
{
	static detail::SFBAudioObjectErrorCategory c;
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

inline std::error_code make_error_code(SFBAudioObjectErrorCode e)
{
	return { static_cast<int>(e), SFBAudioObjectErrorCategory() };
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

/// Throws a @c std::system_error in the @c SFBAudioObjectErrorCategory if @c result!=0
/// @note This is intended for results from the @c AudioObject API
/// @param result An @c OSStatus result code
/// @param operation An optional string describing the operation producing @c result
/// @throw @c std::system_error in the @c SFBAudioObjectErrorCategory
inline void SFBThrowIfAudioObjectError(OSStatus result, const char * const operation = nullptr)
{
	if(__builtin_expect(result != 0, 0))
		throw std::system_error(result, SFBAudioObjectErrorCategory(), operation);
}

/// Throws a @c std::system_error in the @c SFBAudioUnitErrorCategory if @c result!=0
/// @note This is intended for results from the @c AudioUnit API
/// @param result An @c OSStatus result code
/// @param operation An optional string describing the operation producing @c result
/// @throw @c std::system_error in the @c SFBAudioUnitErrorCategory
inline void SFBThrowIfAudioUnitError(OSStatus result, const char * const operation = nullptr)
{
	if(__builtin_expect(result != 0, 0))
		throw std::system_error(result, SFBAudioUnitErrorCategory(), operation);
}

/// Throws a @c std::system_error in the @c SFBAudioCodecErrorCategory if @c result!=0
/// @note This is intended for results from the @c AudioCodec API
/// @param result An @c OSStatus result code
/// @param operation An optional string describing the operation producing @c result
/// @throw @c std::system_error in the @c SFBAudioCodecErrorCategory
inline void SFBThrowIfAudioCodecError(OSStatus result, const char * const operation = nullptr)
{
	if(__builtin_expect(result != 0, 0))
		throw std::system_error(result, SFBAudioCodecErrorCategory(), operation);
}

/// Throws a @c std::system_error in the @c SFBAudioConverterErrorCategory if @c result!=0
/// @note This is intended for results from the @c AudioConverter API
/// @param result An @c OSStatus result code
/// @param operation An optional string describing the operation producing @c result
/// @throw @c std::system_error in the @c SFBAudioConverterErrorCategory
inline void SFBThrowIfAudioConverterError(OSStatus result, const char * const operation = nullptr)
{
	if(__builtin_expect(result != 0, 0))
		throw std::system_error(result, SFBAudioConverterErrorCategory(), operation);
}

/// Throws a @c std::system_error in the @c SFBAudioFileErrorCategory if @c result!=0
/// @note This is intended for results from the @c AudioFile API
/// @param result An @c OSStatus result code
/// @param operation An optional string describing the operation producing @c result
/// @throw @c std::system_error in the @c SFBAudioFileErrorCategory
inline void SFBThrowIfAudioFileError(OSStatus result, const char * const operation = nullptr)
{
	if(__builtin_expect(result != 0, 0))
		throw std::system_error(result, SFBAudioFileErrorCategory(), operation);
}

/// Throws a @c std::system_error in the @c SFBExtAudioFileErrorCategory if @c result!=0
/// @note This is intended for results from the @c ExtAudioFile API
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

	SFBFourCCString(OSStatus errorCode)
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
