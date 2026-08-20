#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <flame_content_sdk_ios/FlameRewardAd.h>

NS_ASSUME_NONNULL_BEGIN
@protocol FlameAdsProvider <NSObject>
- (void)initializeWithAppId:(NSString *)appId appKey:(nullable NSString *)appKey completion:(void (^)(BOOL success, NSString * _Nullable code, NSString * _Nullable desc))completion;
- (nullable id<FlameRewardAd>)createRewardAdWithViewController:(UIViewController *)viewController externalPlacementId:(NSString *)externalPlacementId listener:(nullable id<FlameRewardListener>)listener;
@end
NS_ASSUME_NONNULL_END
