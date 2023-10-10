import XCTest
@testable import CXXAudioUtilities

final class CXXAudioUtilitiesTests: XCTestCase {
	func testByteStream() {
		let bs = SFB.ByteStream()
		XCTAssert(bs.Length() == 0)
		XCTAssert(bs.Remaining() == 0)
		XCTAssert(bs.Position() == 0)
	}
}
