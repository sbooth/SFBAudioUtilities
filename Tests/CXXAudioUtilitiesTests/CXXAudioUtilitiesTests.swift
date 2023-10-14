import XCTest
@testable import CXXAudioUtilities

final class CXXAudioUtilitiesTests: XCTestCase {
	func testByteStream() {
		let bs = SFB.ByteStream()
		XCTAssert(bs.Length() == 0)
		XCTAssert(bs.Remaining() == 0)
		XCTAssert(bs.Position() == 0)
	}

	func testStreamDescription() {
		let asbd = SFB.CAStreamBasicDescription(.float32, 44100, 2, false)
		XCTAssert(asbd.IsPCM() == true)
		XCTAssert(asbd.IsFloat() == true)
		XCTAssert(asbd.IsInteger() == false)
		XCTAssert(asbd.mSampleRate == 44100)
		XCTAssert(asbd.ChannelCount() == 2)
		XCTAssert(asbd.IsInterleaved() == false)
		XCTAssert(asbd.IsNonInterleaved() == true)
	}

	func testChannelLayout() {
		let stereo = SFB.CAChannelLayout.Stereo
		XCTAssert(stereo.ChannelCount() == 2)
	}
}
