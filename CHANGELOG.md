# CHANGELOG

## [0.2.2] - 2026-10-14

### Added

- CSJ Native Interop (migration-compatibility mode) via the new public header
  `FlameCSJInterop.h` (explicit escape hatch, NOT imported by the main umbrella):
  - `FlameCSJInterop.csjNativeInteropReady`: READY = Flame initialized and CSJ
    Content provider started (non-Ads-only, non-simulator).
  - `+prepareUnlockPlayletConfig:onEnterDetail:error:`: atomically turns a
    customer-created `DJXPlayletConfig` into a Flame-managed unlock config
    (Specific mode + per-config unlock delegate + current global free/unlock
    episode counts) before native page creation.
  - `onEnterDetail`: vendor enter-detail hook forwarded per-config with the
    playing drama InfoModel; the host creates/pushes its own native detail page
    (official mixed-stream contract).
- Per-config adapter ownership via associated objects (config -> adapter);
  no lastConfig / global owner inference; customer delegate slots
  (playerDelegate / config.delegate / customViewDelegate) are never touched.

### Compatibility

- Existing init / content entries / components / reward / unlock APIs unchanged;
  customers that never import `FlameCSJInterop.h` are unaffected.
- Unlock business logic fully reused from the existing Bridge/Runtime chain
  (Reward Success != Unlock Success; flowEnd is the authoritative verdict).

### Unsupported (documented contract)

- Duplicate `DJXManager initialize/start` by the host.
- Common-mode native pages requesting Flame-managed unlock.
- Host overriding the Flame-injected interfaceDelegate.
- Mixing Native DrawVideo with Flame Stream/Feed containers (MUST-NOT-MIX).

### Known limitations

- Content runtime is device arm64 only; the simulator slice is intentionally
  Content-disabled.
- Archive-time deployment target override (Xcode 27 toolchain floor is 15.0);
  product support statement unchanged (iOS 13+).
