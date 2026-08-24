# CHANGELOG

## [0.2.0] - 2026-08-24

### Added

- Feed Stream Container public component API for embedded immersive short-drama streams.
- SDK-side initial viewport and safe-area mapping for Host TabBar containment.
- Binary package validation, SHA-256 manifest, and source-free CocoaPods consumer verification workflow.

### Compatibility

- The public initialization, Content, reward, and existing component APIs remain compatible.
- Feed Stream Container is mutually exclusive with Feed Stream Lite and the legacy official stream container.

### Known limitations

- Content runtime is device arm64 only; the simulator slice is intentionally Content-disabled.
- Vendor geometry is configured at initial page creation. UIKit continues to resize child views later,
  but the SDK does not use unverified runtime Vendor geometry mutation APIs.
