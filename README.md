# SFBAudioUtilities

A collection of utility classes and extensions easing common operations in Core Audio, Audio Toolbox, and AVFoundation.


## CoreAudio and AudioToolbox Wrappers

| C++ Class | Description |
| --- | --- |
| [SFB::CABufferList](SFBAudioBufferList.hpp) | A class wrapping a Core Audio `AudioBufferList` with a specific format, frame capacity, and frame length |
| [SFB::CAChannelLayout](SFBAudioChannelLayout.hpp) | A class wrapping a Core Audio `AudioChannelLayout` |
| [SFB::CAPropertyAddress](SFBCAPropertyAddress.hpp) | A class extending the functionality of a Core Audio `AudioObjectPropertyAddress` |
| [SFB::CAStreamBasicDescription](SFBCAStreamBasicDescription.hpp) | A class extending the functionality of a Core Audio `AudioStreamBasicDescription` |
| [SFB::CATimeStamp](SFBCATimeStamp.hpp) | A class extending the functionality of a Core Audio `AudioTimeStamp` |
| [SFB::CAAudioFile](SFBCAAudioFile.hpp) | A wrapper around `AudioFile` |
| [SFB::CAExtAudioFile](SFBCAExtAudioFile.hpp) | A wrapper around `ExtAudioFile` |
| [SFB::HALAudioDevice](SFBHALAudioDevice.hpp) | |
| [SFB::HALAudioObject](SFBHALAudioObject.hpp) | |
| [SFB::HALAudioStream](SFBHALAudioStream.hpp) | |
| [SFB::HALAudioSystemObject](SFBHALAudioSystemObject.hpp) | |

## General Utility Classes

| C++ Class | Description |
| --- | --- |
| [SFB::AudioRingBuffer](SFBAudioRingBuffer.hpp) | A ring buffer supporting non-interleaved audio |
| [SFB::AudioUnitRecorder](SFBAudioUnitRecorder.hpp) | A class that asynchronously writes the output from an `AudioUnit` to a file |
| [SFB::ByteStream](SFBByteStream.hpp) | A `ByteStream` provides heterogeneous typed access to an untyped buffer |
| [SFB::CARingBuffer](SFBCARingBuffer.hpp) | A ring buffer supporting timestamped non-interleaved audio based on Apple's `CARingBuffer` |
| [SFB::CFWrapper](SFBCFWrapper.hpp) | A wrapper around a Core Foundation object |
| [SFB::DeferredClosure](SFBDeferredClosure.hpp) | A class that calls a closure upon destruction |
| [SFB::RingBuffer](SFBRingBuffer.hpp) | A generic ring buffer |
| [SFB::UnfairLock](SFBUnfairLock.hpp) | A wrapper around `os_unfair_lock` implementing C++ `Lockable` |

## AVFoundation Extensions

| Extended Class | Description |
| --- | --- |
| [AVAudioChannelLayout](AVAudioChannelLayout+SFBChannelLabels.h) | Functions for building channel layouts from channel labels |
| [AVAudioFormat](AVAudioFormat+SFBFormatTransformation.h) | Format transformations |
| [AVAudioPCMBuffer](AVAudioPCMBuffer+SFBBufferUtilities.h) | Functions for buffer manipulation |

## Swift Extensions

| Extended Entity | Description |
| --- | --- |
| [AudioChannelLayout](AudioChannelLayout+SFBExtensions.swift) | |
| [AudioStreamBasicDescription](AudioStreamBasicDescription+SFBExtensions.swift) | Common format support |

## License

Released under the [MIT License](https://github.com/sbooth/SFBAudioUtilities/blob/main/LICENSE.txt).
