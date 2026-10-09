# CHANGELOG

## [0.2.1] - 2026-10-09

### Added

- `FlamePrivacyOptions` public API with a new `initWithAppId:appKey:privacyOptions:callback:`
  initialization overload, exposing privacy switches aligned with the CSJ official selectors:
  `canUseLocation`, `allowAccessIDFA`, `canUseWiFiBSSID`, `turnOnTeenMode`, and the
  ICP-filing content filter `isOnlyICPNumber` (aggregate page / feed stream serve only
  ICP-filed dramas when enabled).
- Privacy switches are applied to the GroMore ads stack (privacyProvider / ageGroup /
  mediation IDFA controls) and the CSJ Content stack (`DJXAuthorityConfigDelegate`) before
  their one-shot process initialization.

### Compatibility

- Existing `initWithAppId:appKey:` / `initWithAppId:appKey:callback:` entry points are
  unchanged; passing no options keeps the exact previous behavior.
- Options take effect only on the first real initialization (third-party stacks are
  process-level one-shot); later inputs are ignored with a debug log.

### Known limitations

- Content runtime is device arm64 only; the simulator slice is intentionally Content-disabled.
- `turnOnTeenMode` follows the CSJ official behavior: the Content stack serves no drama
  content while teenager mode is on.
