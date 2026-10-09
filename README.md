# Flame Content SDK 0.2.1

This is the pre-release binary distribution package for Flame Content SDK 0.2.1.
It contains the vendored XCFramework and release metadata only; it does not include SDK source,
customer configuration, test fixtures, CocoaPods caches, or build archives.

## Contents

- `flame_content_sdk_ios.xcframework`: iOS arm64 device slice and arm64/x86_64 simulator slice.
- `Resources/`: resource delivery policy; customer configuration is supplied separately by Flame.
- `Headers/`: public-header delivery policy; canonical headers are inside the XCFramework.
- `flame_content_sdk_ios.podspec`: binary CocoaPods spec for the matching pre-release dist branch.
- `SHA256SUMS`: SHA-256 manifest for all shipped files except the manifest itself.

## Integration

Use the matching `flame-specs` entry and CocoaPods. Do not copy framework slices manually or
initialize third-party Content/Ads SDKs. Content functionality runs on arm64 devices; the simulator
slice intentionally does not contain the Content runtime.

Customer-specific `FlameCustomerConfig.bundle` is a controlled, separate delivery. Never add
`sdk_setting_file.json`, credentials, or user data to this package.
