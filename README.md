# SFBAudioUtilities

A collection of utility classes and extensions easing common operations in Core Audio, Audio Toolbox, and AVFoundation.

The package is organized into three libraries:

| Module Name | Description |
| --- | --- |
| CXXAudioUtilities | C++ classes |
| ObjCSFBAudioUtilities | Objective-C class extensions |
| SFBAudioUtilities | Swift class extensions |

The per-language split is driven by Swift Package Manager's inability to process mixed language targets.

## Installation

Swift Package Manager

Add a package dependency to https://github.com/sbooth/SFBAudioUtilities in Xcode.

Manual or Custom Build

1. Clone the [SFBAudioUtilities](https://github.com/sbooth/SFBAudioUtilities) repository.
2. `swift build`.

## CoreAudio Wrappers

| C++ Class | Description |
| --- | --- |
| [SFB::CABufferList](SFBCABufferList.hpp) | A class wrapping a Core Audio `AudioBufferList` with a specific format, frame capacity, and frame length |
| [SFB::CAChannelLayout](SFBCAChannelLayout.hpp) | A class wrapping a Core Audio `AudioChannelLayout` |
| [SFB::CAPropertyAddress](SFBCAPropertyAddress.hpp) | A class extending the functionality of a Core Audio `AudioObjectPropertyAddress` |
| [SFB::CAStreamBasicDescription](SFBCAStreamBasicDescription.hpp) | A class extending the functionality of a Core Audio `AudioStreamBasicDescription` |
| [SFB::CATimeStamp](SFBCATimeStamp.hpp) | A class extending the functionality of a Core Audio `AudioTimeStamp` |
| [SFB::CAException](SFBCAException.hpp) | `std::error_category` for handling Core Audio errors as exceptions |

### HAL

| C++ Class | Description |
| --- | --- |
| [SFB::HALAudioObject](SFBHALAudioObject.hpp) | A wrapper around a HAL audio object |
| [SFB::HALAudioDevice](SFBHALAudioDevice.hpp) | A wrapper around a HAL audio device |
| [SFB::HALAudioStream](SFBHALAudioStream.hpp) | A wrapper around a HAL audio stream |
| [SFB::HALAudioSystemObject](SFBHALAudioSystemObject.hpp) | A wrapper around `kAudioObjectSystemObject` |

## AudioToolbox Wrappers

| C++ Class | Description |
| --- | --- |
| [SFB::AudioFileWrapper](SFBAudioFileWrapper.hpp) | A bare-bones wrapper around `AudioFile` modeled after `std::unique_ptr` |
| [SFB::ExtAudioFileWrapper](SFBExtAudioFileWrapper.hpp) | A bare-bones wrapper around `ExtAudioFile` modeled after `std::unique_ptr` |
| [SFB::CAAUGraph](SFBCAAUGraph.hpp) | A wrapper around `AUGraph` |
| [SFB::CAAudioFile](SFBCAAudioFile.hpp) | A wrapper around `AudioFile` |
| [SFB::CAExtAudioFile](SFBCAExtAudioFile.hpp) | A wrapper around `ExtAudioFile` |
| [SFB::CAAudioFormat](SFBCAAudioFormat.hpp) | A wrapper around `AudioFormat` |

## Ring Buffers

| C++ Class | Description |
| --- | --- |
| [SFB::RingBuffer](SFBRingBuffer.hpp) | A generic ring buffer |
| [SFB::AudioRingBuffer](SFBAudioRingBuffer.hpp) | A ring buffer supporting non-interleaved audio |
| [SFB::CARingBuffer](SFBCARingBuffer.hpp) | A ring buffer supporting timestamped non-interleaved audio |

## Utility Classes

| C++ Class | Description |
| --- | --- |
| [SFB::ByteStream](SFBByteStream.hpp) | A `ByteStream` provides heterogeneous typed access to an untyped buffer |
| [SFB::CFWrapper](SFBCFWrapper.hpp) | A wrapper around a Core Foundation object |
| [SFB::DeferredClosure](SFBDeferredClosure.hpp) | A class that calls a closure upon destruction |
| [SFB::DispatchSemaphore](SFBDispatchSemaphore.hpp) | A wrapper around `dispatch_semaphore_t` |
| [SFB::UnfairLock](SFBUnfairLock.hpp) | A wrapper around `os_unfair_lock` implementing C++ `Lockable` |

| C++ Class | Description |
| --- | --- |
| [SFB::AudioUnitRecorder](SFBAudioUnitRecorder.hpp) | A class that asynchronously writes the output from an `AudioUnit` to a file |

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
