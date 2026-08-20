#import "../FlameRewardAdBase.h"

NS_ASSUME_NONNULL_BEGIN

/// BUNativeExpressRewardedVideoAd 的 Flame 内部适配器（不暴露给 Host）。
@interface FlameGroMoreRewardAd : FlameRewardAdBase

- (instancetype)initWithViewController:(nullable UIViewController *)viewController
                                slotID:(NSString *)slotID
                              listener:(nullable id<FlameRewardListener>)listener;

@end

NS_ASSUME_NONNULL_END
