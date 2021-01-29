/*
 * Copyright (c) 2021 Stephen F. Booth <me@sbooth.org>
 * MIT license
 */

#pragma once

#import <AudioToolbox/ExtendedAudioFile.h>

#import "SFBCAException.hpp"
#import "SFBAudioChannelLayout.hpp"
#import "SFBAudioStreamBasicDescription.hpp"

namespace {
	struct free_deleter{
		template <typename T>
		void operator()(T *ptr) const {
			std::free(const_cast<std::remove_const_t<T> *>(ptr));
		}
	};
}

class SFBExtAudioFile
{

public:
	inline SFBExtAudioFile() noexcept
	: mExtAudioFile(nullptr)
	{}

	/*! @internal This class is non-copyable */
	SFBExtAudioFile(const SFBExtAudioFile& rhs) = delete;

	/*! @internal This class is non-assignable */
	SFBExtAudioFile& operator=(const SFBExtAudioFile& rhs) = delete;

	~SFBExtAudioFile()
	{
		if(mExtAudioFile)
			ExtAudioFileDispose(mExtAudioFile);
	}

	inline bool IsValid() const noexcept
	{
		return mExtAudioFile != nullptr;
	}

	void OpenURL(CFURLRef inURL)
	{
		Close();
		auto result = ExtAudioFileOpenURL(inURL, &mExtAudioFile);
		SFBCAException::ThrowIfError(result, "ExtAudioFileOpenURL");
	}

	void WrapAudioFileID(AudioFileID inFileID, bool inForWriting)
	{
		Close();
		auto result = ExtAudioFileWrapAudioFileID(inFileID, inForWriting, &mExtAudioFile);
		SFBCAException::ThrowIfError(result, "ExtAudioFileWrapAudioFileID");
	}

	void CreateWithURL(CFURLRef inURL, AudioFileTypeID inFiletype, const AudioStreamBasicDescription& inStreamDesc, const AudioChannelLayout * const inChannelLayout, UInt32 inFlags)
	{
		Close();
		auto result = ExtAudioFileCreateWithURL(inURL, inFiletype, &inStreamDesc, inChannelLayout, inFlags, &mExtAudioFile);
		SFBCAException::ThrowIfError(result, "ExtAudioFileCreateWithURL");
	}

	void Close()
	{
		if(mExtAudioFile) {
			auto result = ExtAudioFileDispose(mExtAudioFile);
			SFBCAException::ThrowIfError(result, "ExtAudioFileDispose");
			mExtAudioFile = nullptr;
		}
	}

	void Read(UInt32& ioNumberFrames, AudioBufferList *ioData)
	{
		auto result = ExtAudioFileRead(mExtAudioFile, &ioNumberFrames, ioData);
		SFBCAException::ThrowIfError(result, "ExtAudioFileRead");
	}

	void Write(UInt32 inNumberFrames, const AudioBufferList *ioData)
	{
		auto result = ExtAudioFileWrite(mExtAudioFile, inNumberFrames, ioData);
		SFBCAException::ThrowIfError(result, "ExtAudioFileWrite");
#if TARGET_OS_IPHONE
			case kExtAudioFileError_CodecUnavailableInputConsumed:
			case kExtAudioFileError_CodecUnavailableInputNotConsumed:
				break;
#endif
	}

	void WriteAsync(UInt32 inNumberFrames, const AudioBufferList *ioData)
	{
		auto result = ExtAudioFileWriteAsync(mExtAudioFile, inNumberFrames, ioData);
		SFBCAException::ThrowIfError(result, "ExtAudioFileOpenURL");
	}

	void Seek(SInt64 inFrameOffset)
	{
		auto result = ExtAudioFileSeek(mExtAudioFile, inFrameOffset);
		SFBCAException::ThrowIfError(result, "ExtAudioFileSeek");
	}

	SInt64 Tell() const
	{
		SInt64 pos;
		auto result = ExtAudioFileTell(mExtAudioFile, &pos);
		SFBCAException::ThrowIfError(result, "ExtAudioFileTell");
		return pos;
	}

	UInt32 GetPropertyInfo(ExtAudioFilePropertyID inPropertyID, Boolean *outWritable) const
	{
		UInt32 size;
		auto result = ExtAudioFileGetPropertyInfo(mExtAudioFile, inPropertyID, &size, outWritable);
		SFBCAException::ThrowIfError(result, "ExtAudioFileGetPropertyInfo");
		return size;
	}

	void GetProperty(ExtAudioFilePropertyID inPropertyID, UInt32& ioPropertyDataSize, void *outPropertyData) const
	{
		auto result = ExtAudioFileGetProperty(mExtAudioFile, inPropertyID, &ioPropertyDataSize, outPropertyData);
		SFBCAException::ThrowIfError(result, "ExtAudioFileGetProperty");
	}

	void SetProperty(ExtAudioFilePropertyID inPropertyID, UInt32 inPropertyDataSize, const void *inPropertyData)
	{
		auto result = ExtAudioFileSetProperty(mExtAudioFile, inPropertyID, inPropertyDataSize, inPropertyData);
		SFBCAException::ThrowIfError(result, "ExtAudioFileSetProperty");
	}

	SFBAudioChannelLayout FileChannelLayout() const
	{
		auto size = GetPropertyInfo(kExtAudioFileProperty_FileChannelLayout, nullptr);
		std::unique_ptr<AudioChannelLayout, free_deleter> layout{static_cast<AudioChannelLayout *>(std::malloc(size))};
		GetProperty(kExtAudioFileProperty_FileChannelLayout, size, layout.get());
		return layout.get();
	}

	void SetFileChannelLayout(const SFBAudioChannelLayout& fileChannelLayout)
	{
		SetProperty(kExtAudioFileProperty_FileChannelLayout, static_cast<UInt32>(fileChannelLayout.Size()), fileChannelLayout);
	}

	SFBAudioStreamBasicDescription FileDataFormat() const
	{
		SFBAudioStreamBasicDescription fileDataFormat;
		UInt32 size = sizeof(fileDataFormat);
		GetProperty(kExtAudioFileProperty_FileDataFormat, size, &fileDataFormat);
		return fileDataFormat;
	}

	SFBAudioStreamBasicDescription ClientDataFormat() const
	{
		SFBAudioStreamBasicDescription clientDataFormat;
		UInt32 size = sizeof(clientDataFormat);
		GetProperty(kExtAudioFileProperty_ClientDataFormat, size, &clientDataFormat);
		return clientDataFormat;
	}

	void SetClientDataFormat(const SFBAudioStreamBasicDescription& clientDataFormat, const SFBAudioChannelLayout * const clientChannelLayout = nullptr, UInt32 codecManufacturer = 0)
	{
		if(codecManufacturer)
			SetProperty(kExtAudioFileProperty_CodecManufacturer, sizeof(codecManufacturer), &codecManufacturer);
		SetProperty(kExtAudioFileProperty_ClientDataFormat, sizeof(clientDataFormat), &clientDataFormat);
		if(clientChannelLayout)
			SetClientChannelLayout(*clientChannelLayout);
	}

	SFBAudioChannelLayout ClientChannelLayout() const
	{
		auto size = GetPropertyInfo(kExtAudioFileProperty_ClientChannelLayout, nullptr);
		std::unique_ptr<AudioChannelLayout, free_deleter> layout{static_cast<AudioChannelLayout *>(std::malloc(size))};
		GetProperty(kExtAudioFileProperty_ClientChannelLayout, size, layout.get());
		return layout.get();
	}

	void SetClientChannelLayout(const SFBAudioChannelLayout& clientChannelLayout)
	{
		SetProperty(kExtAudioFileProperty_ClientChannelLayout, static_cast<UInt32>(clientChannelLayout.Size()), clientChannelLayout);
	}

	AudioConverterRef Converter() const
	{
		UInt32 size = sizeof(AudioConverterRef);
		AudioConverterRef converter = nullptr;
		GetProperty(kExtAudioFileProperty_AudioConverter, size, &converter);
		return converter;
	}

	bool HasConverter() const
	{
		return Converter() != nullptr;
	}

	void SetConverterProperty(AudioConverterPropertyID inPropertyID, UInt32 inPropertyDataSize, const void *inPropertyData)
	{
		auto result = AudioConverterSetProperty(Converter(), inPropertyID, inPropertyDataSize, inPropertyData);
		SFBCAException::ThrowIfError(result, "AudioConverterSetProperty");
		CFPropertyListRef config = nullptr;
		SetProperty(kExtAudioFileProperty_ConverterConfig, sizeof(config), &config);
	}

	SInt64 FrameLength() const
	{
		SInt64 frameLength;
		UInt32 size = sizeof(frameLength);
		GetProperty(kExtAudioFileProperty_FileLengthFrames, size, &frameLength);
		return frameLength;
	}

private:

	ExtAudioFileRef mExtAudioFile;

};