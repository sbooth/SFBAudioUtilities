import XCTest
@testable import CXXAudioUtilities

final class CXXAudioUtilitiesTests: XCTestCase {
	func testCxx() {
		let sysobj = SFB.HALAudioSystemObject()
		let defaultOutput = sysobj.DefaultOutputDevice()
		_ = defaultOutput.Name()
	}
}
