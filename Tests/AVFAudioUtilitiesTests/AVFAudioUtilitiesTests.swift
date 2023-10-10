import XCTest
import AVFAudio
@testable import AVFAudioUtilities

final class AVFAudioUtilitiesTests: XCTestCase {
	func testChannelLabels() {
		let labels: [AudioChannelLabel] = [kAudioChannelLabel_Left, kAudioChannelLabel_Right]
		labels.withUnsafeBufferPointer {
			let layout = AVAudioChannelLayout(channelLabels: $0.baseAddress!, count: AVAudioChannelCount($0.count))
			XCTAssertNotNil(layout)
			XCTAssert(layout!.channelCount == 2)
		}
	}

	func testChannelLabelString() {
		let layout = AVAudioChannelLayout(channelLabelString: "L R")
		XCTAssertNotNil(layout)
		XCTAssert(layout!.channelCount == 2)
	}
}
