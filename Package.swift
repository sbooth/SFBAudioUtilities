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
			name: "AVFAudioExtensions",
			targets: ["AVFAudioExtensions"]),
		.library(
			name: "CoreAudioExtensions",
			targets: ["CoreAudioExtensions"]),
	],
	targets: [
		// Targets are the basic building blocks of a package. A target can define a module or a test suite.
		// Targets can depend on other targets in this package, and on products in packages this package depends on.
		.target(
			name: "CXXAudioUtilities",
			dependencies: [],
			// TODO: Investigate using Swift 5.9 C++ interoperability
			// https://www.swift.org/documentation/cxx-interop/
//			swiftSettings: [.interoperabilityMode(.Cxx)],
			linkerSettings: [
				.linkedFramework("CoreAudio"),
				.linkedFramework("AudioToolbox"),
			]),
		.target(
			name: "AVFAudioExtensions",
			dependencies: [],
			linkerSettings: [
				.linkedFramework("AVFAudio"),
			]),
		.target(
			name: "CoreAudioExtensions",
			dependencies: []),
		// TODO: Add this test if C++ interoperability is enabled
//		.testTarget(
//			name: "CXXAudioUtilitiesTests",
//			dependencies: ["CXXAudioUtilities"],
//			swiftSettings: [.interoperabilityMode(.Cxx)]),
		.testTarget(
			name: "AVFAudioExtensionsTests",
			dependencies: ["AVFAudioExtensions"]),
		.testTarget(
			name: "CoreAudioExtensionsTests",
			dependencies: ["CoreAudioExtensions"]),
	],
	cxxLanguageStandard: .cxx14
)
