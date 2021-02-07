# SFBAudioUtilities

A collection of utility classes and extensions easing common operations in Core Audio, Audio Toolbox, and AVFoundation.


## CoreAudio and AudioToolbox Helper Classes

| C++ Class | Description |
| --- | --- |
| [SFBAudioBufferList](SFBAudioBufferList.hpp) | A class wrapping a Core Audio `AudioBufferList` with a specific format, frame capacity, and frame length |
| [SFBAudioChannelLayout](SFBAudioChannelLayout.hpp) | A class wrapping a Core Audio `AudioChannelLayout` |
| [SFBAudioObjectPropertyAddress](SFBAudioObjectPropertyAddress.hpp) | A class extending the functionality of a Core Audio `AudioObjectPropertyAddress` |
| [SFB::AudioRingBuffer](SFBAudioRingBuffer.hpp) | A ring buffer supporting non-interleaved audio |
| [SFBAudioStreamBasicDescription](SFBAudioStreamBasicDescription.hpp) | A class extending the functionality of a Core Audio `AudioStreamBasicDescription` |
| [SFBAudioTimeStamp](SFBAudioTimeStamp.hpp) | A class extending the functionality of a Core Audio `AudioTimeStamp` |
| [SFBAudioUnitRecorder](SFBAudioUnitRecorder.hpp) | A class that asynchronously writes the output from an `AudioUnit` to a file |
| [SFB::ByteStream](SFBByteStream.hpp) | A `SFBByteStream` provides heterogeneous typed access to an untyped buffer. |
| [SFBCARingBuffer](SFBCARingBuffer.hpp) | A ring buffer supporting timestamped non-interleaved audio based on Apple's `CARingBuffer`. |
| [SFB::CFWrapper](SFBCFWrapper.hpp) | A wrapper around a Core Foundation object|
| [SFB::DeferredClosure](SFBDeferredClosure.hpp) | A class that calls a closure upon destruction. |
| [SFBExtAudioFile](SFBExtAudioFile.hpp) | A wrapper around `ExtAudioFile` |
| [SFBHALAudioDevice](SFBHALAudioDevice.hpp) | |
| [SFBHALAudioObject](SFBHALAudioObject.hpp) | |
| [SFBHALAudioStream](SFBHALAudioStream.hpp) | |
| [SFBHALAudioSystemObject](SFBHALAudioSystemObject.hpp) | |
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
