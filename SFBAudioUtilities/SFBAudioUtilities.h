/*
 * Copyright (c) 2021 Stephen F. Booth <me@sbooth.org>
 * MIT license
 */

#import <Foundation/Foundation.h>

//! Project version number for SFBAudioUtilities.
FOUNDATION_EXPORT double SFBAudioUtilitiesVersionNumber;

//! Project version string for SFBAudioUtilities.
FOUNDATION_EXPORT const unsigned char SFBAudioUtilitiesVersionString[];

#import <SFBAudioUtilities/SFBAudioBufferList.hpp>
#import <SFBAudioUtilities/SFBAudioChannelLayout.hpp>
#import <SFBAudioUtilities/SFBAudioRingBuffer.hpp>
#import <SFBAudioUtilities/SFBAudioStreamBasicDescription.hpp>
#import <SFBAudioUtilities/SFBAudioTimeStamp.hpp>
#import <SFBAudioUtilities/SFBAudioUnitRecorder.hpp>
#import <SFBAudioUtilities/SFBCAException.hpp>
#import <SFBAudioUtilities/SFBCARingBuffer.hpp>
#import <SFBAudioUtilities/SFBCFWrapper.hpp>
#import <SFBAudioUtilities/SFBDeferredClosure.hpp>
#import <SFBAudioUtilities/SFBExtAudioFile.hpp>
#import <SFBAudioUtilities/SFBRingBuffer.hpp>
#import <SFBAudioUtilities/SFBUnfairLock.hpp>
