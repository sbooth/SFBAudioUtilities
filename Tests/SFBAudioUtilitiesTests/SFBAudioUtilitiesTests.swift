import XCTest
@testable import SFBAudioUtilities

final class SFBAudioUtilitiesTests: XCTestCase {
	func testAudioStreamBasicDescription() {
		let asbd = AudioStreamBasicDescription(commonFormat: .float32, sampleRate: 44100, channelsPerFrame: 2, isInterleaved: false)
		XCTAssertTrue(asbd.isFloat)
		XCTAssertTrue(asbd.isNonInterleaved)
		XCTAssertTrue(asbd.channelCount == 2)
		XCTAssertTrue(asbd.channelStreamCount == 2)
	}
}
