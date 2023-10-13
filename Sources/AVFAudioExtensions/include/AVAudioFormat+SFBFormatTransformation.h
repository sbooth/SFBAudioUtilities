//
// Copyright (c) 2020 - 2023 Stephen F. Booth <me@sbooth.org>
// Part of https://github.com/sbooth/SFBAudioUtilities
// MIT license
//

#import <AVFAudio/AVFAudio.h>

NS_ASSUME_NONNULL_BEGIN

@interface AVAudioFormat (SFBFormatTransformation)
/// Returns a copy of @c self converted to the equivalent non-interleaved format
/// @note Returns @c nil for non-PCM formats
- (nullable AVAudioFormat *)nonInterleavedEquivalent;
/// Returns a copy of @c self converted to the equivalent interleaved format
/// @note Returns @c nil for non-PCM formats
- (nullable AVAudioFormat *)interleavedEquivalent;
/// Returns a copy of @c self converted to the equivalent standard format
- (nullable AVAudioFormat *)standardEquivalent;
/// Returns the specified common format with the same sample rate and channel layout as @c self
- (nullable AVAudioFormat *)transformedToCommonFormat:(AVAudioCommonFormat)commonFormat interleaved:(BOOL)interleaved;
@end

NS_ASSUME_NONNULL_END
