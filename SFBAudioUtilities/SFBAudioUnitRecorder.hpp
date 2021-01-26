/*
 * Copyright (c) 2021 Stephen F. Booth <me@sbooth.org>
 * MIT license
 */

#pragma once

#import <AudioToolbox/ExtendedAudioFile.h>

class SFBAudioUnitRecorder
{

public:
	
	SFBAudioUnitRecorder(AudioUnit au, CFURLRef outputFileURL, AudioFileTypeID fileType, const AudioStreamBasicDescription& format, UInt32 busNumber = 0)
	: mFileIsOpen(false), mClientFormatIsSet(false), mAudioUnit(au), mExtAudioFile(nullptr), mBusNumber(busNumber)
	{
		assert(au != nullptr);
		OSStatus result = ExtAudioFileCreateWithURL(outputFileURL, fileType, &format, nullptr, kAudioFileFlags_EraseFile, &mExtAudioFile);
		assert(result == noErr);
		mFileIsOpen = true;
	}

	~SFBAudioUnitRecorder()
	{
		if(mExtAudioFile) {
			OSStatus result = ExtAudioFileDispose(mExtAudioFile);
			assert(result == noErr);
			mFileIsOpen = false;
			mExtAudioFile = nullptr;
		}
	}

	void Start() {
		if(mFileIsOpen) {
			if(!mClientFormatIsSet) {
				AudioStreamBasicDescription clientFormat;
				UInt32 size = sizeof(clientFormat);
				OSStatus result = AudioUnitGetProperty(mAudioUnit, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Output, mBusNumber, &clientFormat, &size);
				assert(result == noErr);
				result = ExtAudioFileSetProperty(mExtAudioFile, kExtAudioFileProperty_ClientDataFormat, size, &clientFormat);
				assert(result == noErr);
				mClientFormatIsSet = true;
			}
			OSStatus result = ExtAudioFileWriteAsync(mExtAudioFile, 0, nullptr);
			assert(result == noErr);
			result = AudioUnitAddRenderNotify(mAudioUnit, RenderCallback, this);
			assert(result == noErr);
		}
	}

	void Stop() {
		if(mFileIsOpen) {
			OSStatus result = AudioUnitRemoveRenderNotify(mAudioUnit, RenderCallback, this);
			assert(result == noErr);
		}
	}

private:

	static OSStatus RenderCallback(void *inRefCon, AudioUnitRenderActionFlags *ioActionFlags, const AudioTimeStamp *inTimeStamp, UInt32 inBusNumber, UInt32 inNumberFrames, AudioBufferList *ioData)
	{
		SFBAudioUnitRecorder *THIS = static_cast<SFBAudioUnitRecorder *>(inRefCon);
		if(*ioActionFlags & kAudioUnitRenderAction_PostRender) {
			if(THIS->mBusNumber == inBusNumber && !(*ioActionFlags & kAudioUnitRenderAction_PostRenderError)) {
				OSStatus result = ExtAudioFileWriteAsync(THIS->mExtAudioFile, inNumberFrames, ioData);
				assert(result == noErr);
			}
		}
		return noErr;
	}

	bool				mFileIsOpen;
	bool				mClientFormatIsSet;
	AudioUnit			mAudioUnit;
	ExtAudioFileRef		mExtAudioFile;
	UInt32				mBusNumber;

};
