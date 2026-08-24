#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@class FlameContentContainerViewController;

typedef NS_ENUM(NSInteger, FlameContentContainerLifecycleEvent) {
    FlameContentContainerLifecycleEventAppear,
    FlameContentContainerLifecycleEventDisappear,
    FlameContentContainerLifecycleEventBackground,
    FlameContentContainerLifecycleEventForeground,
    FlameContentContainerLifecycleEventPause,
    FlameContentContainerLifecycleEventResume,
};

@protocol FlameContentContainerLifecycleObserver <NSObject>

@optional
- (void)contentContainer:(FlameContentContainerViewController *)container
       didReceiveEvent:(FlameContentContainerLifecycleEvent)event;

@end

/// Host-owned Feed component container.
///
/// Create this controller only with `FlameContentSdk.createFeedContainerViewController`.
/// Add it to a host controller with standard UIKit child-controller containment. Phase 1 supports
/// one live Feed container at a time; creating a second container returns nil until the current
/// container is released.
@interface FlameContentContainerViewController : UIViewController

@property (nonatomic, weak, nullable) id<FlameContentContainerLifecycleObserver> lifecycleObserver;

- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

@end

NS_ASSUME_NONNULL_END
