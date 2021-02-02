//
// Copyright (c) 2021 Stephen F. Booth <me@sbooth.org>
// MIT license
//

#pragma once

#import <AudioToolbox/AudioFile.h>

#import "SFBCAException.hpp"
#import "SFBAudioStreamBasicDescription.hpp"

CF_ASSUME_NONNULL_BEGIN

/// A wrapper around @c AudioFile
class SFBAudioFile
{

private:

	struct free_deleter {
		template <typename T>
		void operator()(T * _Nonnull ptr) const {
			std::free(const_cast<std::remove_const_t<T> *>(ptr));
		}
	};

public:
	/// Creates a @c SFBAudioFile
	inline SFBAudioFile() noexcept
	: mAudioFileID(nullptr)
	{}

	// This class is non-copyable
	SFBAudioFile(const SFBAudioFile& rhs) = delete;

	// This class is non-assignable
	SFBAudioFile& operator=(const SFBAudioFile& rhs) = delete;

	/// Destroys the @c SFBAudioFile and release all associated resources.
	inline ~SFBAudioFile()
	{
		if(mAudioFileID)
			AudioFileClose(mAudioFileID);
	}

	// This class is non-movable
	SFBAudioFile(SFBAudioFile&& rhs) = delete;

	// This class is non-move assignable
	SFBAudioFile& operator=(SFBAudioFile&& rhs) = delete;

	/// Returns @c true if this object's internal @c ExtAudioFileRef is not @c nullptr
	inline explicit operator bool() const noexcept
	{
		return mAudioFileID != nullptr;
	}

	/// Returns @c true if this object's internal @c ExtAudioFileRef is @c nullptr
	inline bool operator!() const noexcept
	{
		return !mAudioFileID;
	}

	/// Returns @c true if this object's internal @c ExtAudioFileRef is not @c nullptr
	inline bool IsValid() const noexcept
	{
		return this->operator bool();
	}

	/// Returns the file's internal @c AudioFileID
	operator AudioFileID const _Nullable () const
	{
		return mAudioFileID;
	}

	/// Opens an existing audio file.
	/// @throw @c std::system_error
	void OpenURL(CFURLRef inURL, AudioFilePermissions inPermissions, AudioFileTypeID inFileTypeHint)
	{
		Close();
		auto result = AudioFileOpenURL(inURL, inPermissions, inFileTypeHint, &mAudioFileID);
		SFBThrowIfAudioFileError(result, "AudioFileOpenURL");
	}

	/// Creates a new audio file  (or initialises an existing file).
	/// @throw @c std::system_error
	void CreateWithURL(CFURLRef inURL, AudioFileTypeID inFileType, const AudioStreamBasicDescription& inFormat, AudioFileFlags inFlags)
	{
		Close();
		auto result = AudioFileCreateWithURL(inURL, inFileType, &inFormat, inFlags, &mAudioFileID);
		SFBThrowIfAudioFileError(result, "AudioFileCreateWithURL");
	}

	/// Wipes clean an existing file. You provide callbacks that the AudioFile API will use to get the data.
	/// @throw @c std::system_error
	void InitializeWithCallbacks(void *inClientData, AudioFile_ReadProc inReadFunc, AudioFile_WriteProc inWriteFunc, AudioFile_GetSizeProc inGetSizeFunc, AudioFile_SetSizeProc inSetSizeFunc, AudioFileTypeID inFileType, const AudioStreamBasicDescription& inFormat, AudioFileFlags inFlags)
	{
		Close();
		auto result = AudioFileInitializeWithCallbacks(inClientData, inReadFunc, inWriteFunc, inGetSizeFunc, inSetSizeFunc, inFileType, &inFormat, inFlags, &mAudioFileID);
		SFBThrowIfAudioFileError(result, "AudioFileInitializeWithCallbacks");
	}

	/// Opens an existing file. You provide callbacks that the AudioFile API will use to get the data.
	/// @throw @c std::system_error
	void OpenWithCallbacks(void *inClientData, AudioFile_ReadProc inReadFunc, AudioFile_WriteProc inWriteFunc, AudioFile_GetSizeProc inGetSizeFunc, AudioFile_SetSizeProc inSetSizeFunc, AudioFileTypeID inFileType, const AudioStreamBasicDescription& inFormat, AudioFileFlags inFlags)
	{
		Close();
		auto result = AudioFileInitializeWithCallbacks(inClientData, inReadFunc, inWriteFunc, inGetSizeFunc, inSetSizeFunc, inFileType, &inFormat, inFlags, &mAudioFileID);
		SFBThrowIfAudioFileError(result, "AudioFileInitializeWithCallbacks");
	}

	/// Close an existing audio file.
	/// @throw @c std::system_error
	void Close()
	{
		if(mAudioFileID) {
			auto result = AudioFileClose(mAudioFileID);
			SFBThrowIfAudioFileError(result, "AudioFileClose");
			mAudioFileID = nullptr;
		}
	}

	/// Moves the audio data to the end of the file and other internal optimizations of the file structure.
	/// @throw @c std::system_error
	void Optimize()
	{
		auto result = AudioFileOptimize(mAudioFileID);
		SFBThrowIfAudioFileError(result, "AudioFileOptimize");
	}

	/// Reads bytes of audio data from the audio file.
	/// @throw @c std::system_error
	OSStatus ReadBytes(bool inUseCache, SInt64 inStartingByte, UInt32& ioNumBytes, void *outBuffer)
	{
		auto result = AudioFileReadBytes(mAudioFileID, inUseCache, inStartingByte, &ioNumBytes, outBuffer);
		switch(result) {
			case noErr:
			case kAudioFileEndOfFileError:
				break;
			default:
				SFBThrowIfAudioFileError(result, "AudioFileReadBytes");
				break;
		}
		return result;
	}

	/// Writes bytes of audio data to the audio file.
	/// @throw @c std::system_error
	void WriteBytes(bool inUseCache, SInt64 inStartingByte, UInt32& ioNumBytes, const void *inBuffer)
	{
		auto result = AudioFileWriteBytes(mAudioFileID, inUseCache, inStartingByte, &ioNumBytes, inBuffer);
		SFBThrowIfAudioFileError(result, "AudioFileWriteBytes");
	}

	/// Reads packets of audio data from the audio file.
	/// @throw @c std::system_error
	OSStatus ReadPacketData(bool inUseCache, UInt32& ioNumBytes, AudioStreamPacketDescription * __nullable outPacketDescriptions, SInt64 inStartingPacket, UInt32& ioNumPackets, void * __nullable outBuffer)
	{
		auto result = AudioFileReadPacketData(mAudioFileID, inUseCache, &ioNumBytes, outPacketDescriptions, inStartingPacket, &ioNumPackets, outBuffer);
		switch(result) {
			case noErr:
			case kAudioFileEndOfFileError:
				break;
			default:
				SFBThrowIfAudioFileError(result, "AudioFileReadPacketData");
				break;
		}
		return result;
	}

	/// Writes packets of audio data to the audio file.
	/// @throw @c std::system_error
	void WritePackets(bool inUseCache, UInt32 inNumBytes, const AudioStreamPacketDescription * __nullable inPacketDescriptions, SInt64 inStartingPacket, UInt32& ioNumPackets, const void *inBuffer)
	{
		auto result = AudioFileWritePackets(mAudioFileID, inUseCache, inNumBytes, inPacketDescriptions, inStartingPacket, &ioNumPackets, inBuffer);
		SFBThrowIfAudioFileError(result, "AudioFileWritePackets");
	}

	/// Gets the size of user data in a file.
	/// @throw @c std::system_error
	UInt32 GetUserDataSize(UInt32 inUserDataID, UInt32 inIndex)
	{
		UInt32 size;
		auto result = AudioFileGetUserDataSize(mAudioFileID, inUserDataID, inIndex, &size);
		SFBThrowIfAudioFileError(result, "AudioFileGetUserDataSize");
		return size;
	}

	/// Gets the data of a chunk in a file.
	/// @throw @c std::system_error
	void GetUserData(UInt32 inUserDataID, UInt32 inIndex, UInt32& ioUserDataSize, void *outUserData)
	{
		auto result = AudioFileGetUserData(mAudioFileID, inUserDataID, inIndex, &ioUserDataSize, outUserData);
		SFBThrowIfAudioFileError(result, "AudioFileGetUserData");
	}

	/// Sets the data of a chunk in a file.
	/// @throw @c std::system_error
	void SetUserData(UInt32 inUserDataID, UInt32 inIndex, UInt32 inUserDataSize, const void *inUserData)
	{
		auto result = AudioFileSetUserData(mAudioFileID, inUserDataID, inIndex, inUserDataSize, inUserData);
		SFBThrowIfAudioFileError(result, "AudioFileGetUserData");
	}

	/// Removes a user chunk in a file.
	/// @throw @c std::system_error
	void RemoveUserData(UInt32 inUserDataID, UInt32 inIndex)
	{
		auto result = AudioFileRemoveUserData(mAudioFileID, inUserDataID, inIndex);
		SFBThrowIfAudioFileError(result, "AudioFileRemoveUserData");
	}

	/// Gets information about the size of a property of an AudioFile  and whether it can be set.
	/// @throw @c std::system_error
	UInt32 GetPropertyInfo(AudioFilePropertyID inPropertyID, UInt32 * _Nullable isWritable) const
	{
		UInt32 size;
		auto result = AudioFileGetPropertyInfo(mAudioFileID, inPropertyID, &size, isWritable);
		SFBThrowIfAudioFileError(result, "AudioFileGetPropertyInfo");
		return size;
	}

	/// Copies the value for a property of an AudioFile into a buffer.
	/// @throw @c std::system_error
	void GetProperty(AudioFilePropertyID inPropertyID, UInt32& ioDataSize, void *outPropertyData) const
	{
		auto result = AudioFileGetProperty(mAudioFileID, inPropertyID, &ioDataSize, outPropertyData);
		SFBThrowIfAudioFileError(result, "AudioFileGetProperty");
	}

	/// Sets the value for a property of an AudioFile.
	/// @throw @c std::system_error
	void SetProperty(AudioFilePropertyID inPropertyID, UInt32 inDataSize, const void *inPropertyData)
	{
		auto result = AudioFileSetProperty(mAudioFileID, inPropertyID, inDataSize, inPropertyData);
		SFBThrowIfAudioFileError(result, "AudioFileSetProperty");
	}

	/// Returns the file's  format (@c kAudioFilePropertyFileFormat)
	/// @throw @c std::system_error
	AudioFileTypeID FileFormat() const
	{
		AudioFileTypeID fileFormat;
		UInt32 size = sizeof(fileFormat);
		GetProperty(kAudioFilePropertyFileFormat, size, &fileFormat);
		return fileFormat;
	}

	/// Returns the file's data format (@c kAudioFilePropertyDataFormat)
	/// @throw @c std::system_error
	SFBAudioStreamBasicDescription FileDataFormat() const
	{
		SFBAudioStreamBasicDescription fileDataFormat;
		UInt32 size = sizeof(fileDataFormat);
		GetProperty(kAudioFilePropertyDataFormat, size, &fileDataFormat);
		return fileDataFormat;
	}

#pragma mark Global Properties

	/// Gets information about the size of a property of an AudioFile  and whether it can be set.
	/// @throw @c std::system_error
	static UInt32 GetGlobalInfoSize(AudioFilePropertyID inPropertyID, UInt32 inSpecifierSize, void * __nullable inSpecifier)
	{
		UInt32 size;
		auto result = AudioFileGetGlobalInfoSize(inPropertyID, inSpecifierSize, inSpecifier, &size);
		SFBThrowIfAudioFileError(result, "AudioFileGetGlobalInfoSize");
		return size;
	}

	/// Copies the value for a property of an AudioFile into a buffer.
	/// @throw @c std::system_error
	static void GetGlobalInfo(AudioFilePropertyID inPropertyID, UInt32 inSpecifierSize, void * __nullable inSpecifier, UInt32& ioDataSize, void *outPropertyData)
	{
		auto result = AudioFileGetGlobalInfo(inPropertyID, inSpecifierSize, inSpecifier, &ioDataSize, outPropertyData);
		SFBThrowIfAudioFileError(result, "AudioFileGetGlobalInfo");
	}

	/// Returns an array of UInt32 containing the file types (i.e. AIFF, WAVE, etc) that can be opened for reading.
	/// @throw @c std::system_error
	static std::vector<AudioFileTypeID> ReadableTypes()
	{
		auto size = GetGlobalInfoSize(kAudioFileGlobalInfo_ReadableTypes, 0, nullptr);
		auto count = size / sizeof(AudioFileTypeID);
		auto types = std::vector<AudioFileTypeID>(count);
		GetGlobalInfo(kAudioFileGlobalInfo_ReadableTypes, 0, nullptr, size, &types[0]);
		return types;
	}

	/// Returns an array of UInt32 containing the file types (i.e. AIFF, WAVE, etc) that can be opened for writing.
	/// @throw @c std::system_error
	static std::vector<AudioFileTypeID> WritableTypes()
	{
		auto size = GetGlobalInfoSize(kAudioFileGlobalInfo_WritableTypes, 0, nullptr);
		auto count = size / sizeof(AudioFileTypeID);
		auto types = std::vector<AudioFileTypeID>(count);
		GetGlobalInfo(kAudioFileGlobalInfo_WritableTypes, 0, nullptr, size, &types[0]);
		return types;
	}

private:

	/// The underlying @c AudioFile object
	AudioFileID _Nullable mAudioFileID;

};

CF_ASSUME_NONNULL_END
