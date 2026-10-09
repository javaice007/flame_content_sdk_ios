#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <flame_content_sdk_ios/FlameRewardAd.h>
#import <flame_content_sdk_ios/FlamePrivacyOptions.h>

NS_ASSUME_NONNULL_BEGIN
@protocol FlameAdsProvider <NSObject>
/// privacyOptions 为 nil 表示 Host 未传（保持三方默认）；仅首次真实初始化生效（进程级一次）。
- (void)initializeWithAppId:(NSString *)appId
                     appKey:(nullable NSString *)appKey
              privacyOptions:(nullable FlamePrivacyOptions *)privacyOptions
                 completion:(void (^)(BOOL success, NSString * _Nullable code, NSString * _Nullable desc))completion;
- (nullable id<FlameRewardAd>)createRewardAdWithViewController:(UIViewController *)viewController externalPlacementId:(NSString *)externalPlacementId listener:(nullable id<FlameRewardListener>)listener;
@end
NS_ASSUME_NONNULL_END
