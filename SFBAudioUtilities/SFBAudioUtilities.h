//
// Copyright (c) 2013 - 2022 Stephen F. Booth <me@sbooth.org>
// Part of https://github.com/sbooth/SFBAudioUtilities
// MIT license
//

#import <Foundation/Foundation.h>

//! Project version number for SFBAudioUtilities.
FOUNDATION_EXPORT double SFBAudioUtilitiesVersionNumber;

//! Project version string for SFBAudioUtilities.
FOUNDATION_EXPORT const unsigned char SFBAudioUtilitiesVersionString[];

#ifdef __cplusplus
#import <SFBAudioUtilities/SFBAudioFileWrapper.hpp>
#import <SFBAudioUtilities/SFBAudioRingBuffer.hpp>
#import <SFBAudioUtilities/SFBAudioUnitRecorder.hpp>
#import <SFBAudioUtilities/SFBByteStream.hpp>
#import <SFBAudioUtilities/SFBCAAudioFile.hpp>
#import <SFBAudioUtilities/SFBCAAudioFormat.hpp>
#import <SFBAudioUtilities/SFBCABufferList.hpp>
#import <SFBAudioUtilities/SFBCAChannelLayout.hpp>
#import <SFBAudioUtilities/SFBCAException.hpp>
#import <SFBAudioUtilities/SFBCAExtAudioFile.hpp>
#import <SFBAudioUtilities/SFBCAPropertyAddress.hpp>
#import <SFBAudioUtilities/SFBCARingBuffer.hpp>
#import <SFBAudioUtilities/SFBCAStreamBasicDescription.hpp>
#import <SFBAudioUtilities/SFBCATimeStamp.hpp>
#import <SFBAudioUtilities/SFBCFWrapper.hpp>
#import <SFBAudioUtilities/SFBDeferredClosure.hpp>
#import <SFBAudioUtilities/SFBExtAudioFileWrapper.hpp>
#import <SFBAudioUtilities/SFBHALAudioDevice.hpp>
#import <SFBAudioUtilities/SFBHALAudioObject.hpp>
#import <SFBAudioUtilities/SFBHALAudioStream.hpp>
#import <SFBAudioUtilities/SFBHALAudioSystemObject.hpp>
#import <SFBAudioUtilities/SFBRingBuffer.hpp>
#import <SFBAudioUtilities/SFBUnfairLock.hpp>
#endif /* __cplusplus */

#ifdef __OBJC__
#import <SFBAudioUtilities/AVAudioChannelLayout+SFBChannelLabels.h>
#import <SFBAudioUtilities/AVAudioFormat+SFBFormatTransformation.h>
#import <SFBAudioUtilities/AVAudioPCMBuffer+SFBBufferUtilities.h>
#endif /* __OBJC__ */
