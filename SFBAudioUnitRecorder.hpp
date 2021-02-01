//
// Copyright (c) 2021 Stephen F. Booth <me@sbooth.org>
// MIT license
//

#pragma once

#import <stdexcept>

#import <os/log.h>

#import <AudioToolbox/ExtendedAudioFile.h>

#import "SFBExtAudioFile.hpp"

CF_ASSUME_NONNULL_BEGIN

/// A class that asynchronously writes the output from an @c AudioUnit to a file
class SFBAudioUnitRecorder
{

public:

	/// Creates a new @c SFBAudioUnitRecorder that asynchronously writes the output from an @c AudioUnit to a file
	/// @param au The @c AudioUnit to record
	/// @param outputFileURL The URL of the output audio file
	/// @param fileType The type of the file to create
	/// @param format The format of the audio data to be written to the file
	/// @param busNumber The bus number of @c au to record
	SFBAudioUnitRecorder(AudioUnit au, CFURLRef outputFileURL, AudioFileTypeID fileType, const AudioStreamBasicDescription& format, UInt32 busNumber = 0)
	: mClientFormatIsSet(false), mAudioUnit(au), mBusNumber(busNumber)
	{
		if(!au)
			throw std::invalid_argument("au == nullptr");
		mExtAudioFile.CreateWithURL(outputFileURL, fileType, format, nullptr, kAudioFileFlags_EraseFile);
	}

	/// Starts recording
	void Start() {
		if(mExtAudioFile.IsValid()) {
			if(!mClientFormatIsSet) {
				AudioStreamBasicDescription clientFormat;
				UInt32 size = sizeof(clientFormat);
				OSStatus result = AudioUnitGetProperty(mAudioUnit, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Output, mBusNumber, &clientFormat, &size);
				SFBThrowIfAudioUnitError(result, "AudioUnitGetProperty");
				mExtAudioFile.SetClientDataFormat(clientFormat);
				mClientFormatIsSet = true;
			}
			mExtAudioFile.WriteAsync(0, nullptr);
			auto result = AudioUnitAddRenderNotify(mAudioUnit, RenderCallback, this);
			SFBThrowIfAudioUnitError(result, "AudioUnitAddRenderNotify");
		}
	}

	/// Stops recording
	void Stop() {
		if(mExtAudioFile.IsValid()) {
			OSStatus result = AudioUnitRemoveRenderNotify(mAudioUnit, RenderCallback, this);
			SFBThrowIfAudioUnitError(result, "AudioUnitRemoveRenderNotify");
		}
	}

private:

	/// Asynchronously writes rendered audio to the file
	static OSStatus RenderCallback(void *inRefCon, AudioUnitRenderActionFlags *ioActionFlags, const AudioTimeStamp *inTimeStamp, UInt32 inBusNumber, UInt32 inNumberFrames, AudioBufferList *ioData)
	{
		SFBAudioUnitRecorder *THIS = static_cast<SFBAudioUnitRecorder *>(inRefCon);
		if(*ioActionFlags & kAudioUnitRenderAction_PostRender) {
			if(THIS->mBusNumber == inBusNumber && !(*ioActionFlags & kAudioUnitRenderAction_PostRenderError)) {
				try {
					THIS->mExtAudioFile.WriteAsync(inNumberFrames, ioData);
				}
				catch(const std::exception& e) {
					os_log_debug(OS_LOG_DEFAULT, "Error writing frames: %{public}s", e.what());
				}
			}
		}
		return noErr;
	}

	/// The underlying @c ExtAudioFile
	SFBExtAudioFile mExtAudioFile;
	/// @c true if the @c ExtAudioFile client format is set
	bool mClientFormatIsSet;
	/// The @c AudioUnit to record
	AudioUnit mAudioUnit;
	/// The bus number of @c mAudioUnit to record
	UInt32 mBusNumber;

};

CF_ASSUME_NONNULL_END
