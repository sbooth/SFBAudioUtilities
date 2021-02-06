# SFBAudioUtilities

A collection of utility classes and extensions easing common operations in Core Audio, Audio Toolbox, and AVFoundation.


## CoreAudio and AudioToolbox

| C++ Class | Description |
| --- | --- |
| SFBAudioBufferList | A class wrapping a Core Audio `AudioBufferList` with a specific format, frame capacity, and frame length |
| SFBAudioChannelLayout | A class wrapping a Core Audio `AudioChannelLayout` |
| SFBAudioObjectPropertyAddress | A class extending the functionality of a Core Audio `AudioObjectPropertyAddress` |
| SFBAudioRingBuffer | A ring buffer supporting non-interleaved audio |
| SFBAudioStreamBasicDescription | A class extending the functionality of a Core Audio `AudioStreamBasicDescription` |
| SFBAudioTimeStamp | A class extending the functionality of a Core Audio `AudioTimeStamp` |
| SFBAudioUnitRecorder | A class that asynchronously writes the output from an `AudioUnit` to a file |
| SFBByteStream | A `SFBByteStream` provides heterogeneous typed access to an untyped buffer. |
| SFBCARingBuffer | A ring buffer supporting timestamped non-interleaved audio based on Apple's `CARingBuffer`. |
| SFBCFWrapper | A wrapper around a Core Foundation object|
| SFBExtAudioFile | A wrapper around `ExtAudioFile` |
| SFBHALAudioDevice | |
| SFBHALAudioObject | |
| SFBHALAudioStream | |
| SFBHALAudioSystemObject | |
| SFBRingBuffer | A ring buffer |
| SFBUnfairLock | A wrapper around `os_unfair_lock` implementing C++ `Lockable` |

## AVFoundation

| Extended Class | Description |
| --- | --- |
| AVAudioChannelLayout | Functions for building channel layouts from channel labels |
| AVAudioFormat | Format transformations |
| AVAudioPCMBuffer | Functions for buffer manipulation |

## Swift Extensions

| Extended Entity | Description |
| --- | --- |
| AudioChannelLayout | |
| AudioStreamBasicDescription | Common format support |

## License

Released under the [MIT License](https://github.com/sbooth/SFBAudioUtilities/blob/master/LICENSE.txt).
