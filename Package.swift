// swift-tools-version: 5.6
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "SFBAudioUtilities",
    products: [
        // Products define the executables and libraries a package produces, and make them visible to other packages.
        .library(
            name: "CXXAudioUtilities",
            targets: ["CXXAudioUtilities"]),
		.library(
			name: "AVFAudioUtilities",
			targets: ["AVFAudioUtilities"]),
		.library(
			name: "CoreAudioUtilities",
			targets: ["CoreAudioUtilities"]),
    ],
    targets: [
        // Targets are the basic building blocks of a package. A target can define a module or a test suite.
        // Targets can depend on other targets in this package, and on products in packages this package depends on.
        .target(
            name: "CXXAudioUtilities",
            dependencies: [],
			linkerSettings: [.linkedFramework("AudioToolbox")]),
		.target(
			name: "AVFAudioUtilities",
			dependencies: []),
		.target(
			name: "CoreAudioUtilities",
			dependencies: []),
		// C++ interop requires Swift 5.9
//        .testTarget(
//            name: "CXXAudioUtilitiesTests",
//            dependencies: ["CXXAudioUtilities"],
//			swiftSettings: [.interoperabilityMode(.Cxx)]),
		.testTarget(
			name: "AVFAudioUtilitiesTests",
			dependencies: ["AVFAudioUtilities"]),
		.testTarget(
			name: "CoreAudioUtilitiesTests",
			dependencies: ["CoreAudioUtilities"]),
    ],
    cxxLanguageStandard: .cxx17
)
