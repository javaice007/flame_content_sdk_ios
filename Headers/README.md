# Public Headers

Canonical public headers and `module.modulemap` are packaged inside each XCFramework slice:

- `flame_content_sdk_ios.h`
- `FlameContentSdk.h`
- `FlameCallback.h`
- `FlameRewardAd.h`
- `FlameContentContainerViewController.h`
- `FlameContentEntry.h`
- `FlameContentAuxiliary.h`

Do not maintain a second copied header tree outside the XCFramework. Import the umbrella header:

```objc
#import <flame_content_sdk_ios/flame_content_sdk_ios.h>
```
