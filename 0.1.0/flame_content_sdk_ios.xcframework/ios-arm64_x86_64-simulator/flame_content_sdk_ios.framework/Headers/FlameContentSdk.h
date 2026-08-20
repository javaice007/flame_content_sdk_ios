#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <flame_content_sdk_ios/FlameCallback.h>
#import <flame_content_sdk_ios/FlameRewardAd.h>

NS_ASSUME_NONNULL_BEGIN

@interface FlameContentSdk : NSObject

+ (instancetype)sharedInstance;

+ (void)initWithAppId:(NSString *)appId appKey:(NSString *)appKey;
+ (void)initWithAppId:(NSString *)appId appKey:(NSString *)appKey callback:(nullable id<FlameCallback>)callback;
+ (void)clear;
+ (void)setDebug:(BOOL)isDebug;
+ (BOOL)isInitialized;
+ (BOOL)checkInitialization;

- (NSString *)getVersion;

/// 创建激励广告对象（Flame Placement ID；内部映射与第三方广告位对 Host 不可见）。
+ (nullable id<FlameRewardAd>)createRewardAdWithViewController:(UIViewController *)viewController
                                                   placementId:(NSString *)placementId
                                                      listener:(nullable id<FlameRewardListener>)listener;

@end

NS_ASSUME_NONNULL_END
