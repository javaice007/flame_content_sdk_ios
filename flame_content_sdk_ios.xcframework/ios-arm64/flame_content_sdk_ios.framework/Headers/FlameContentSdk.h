#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <flame_content_sdk_ios/FlameCallback.h>
#import <flame_content_sdk_ios/FlameRewardAd.h>
#import <flame_content_sdk_ios/FlamePrivacyOptions.h>

NS_ASSUME_NONNULL_BEGIN

@interface FlameContentSdk : NSObject

+ (instancetype)sharedInstance;

+ (void)initWithAppId:(NSString *)appId appKey:(NSString *)appKey;
+ (void)initWithAppId:(NSString *)appId appKey:(NSString *)appKey callback:(nullable id<FlameCallback>)callback;

/// 带隐私开关的初始化（0.2.1）。privacyOptions 传 nil 等价于上一个方法。
/// options 仅在首次真实初始化时生效（三方栈进程级一次）；已 Initialized 后传入会被忽略。
+ (void)initWithAppId:(NSString *)appId
               appKey:(NSString *)appKey
        privacyOptions:(nullable FlamePrivacyOptions *)privacyOptions
              callback:(nullable id<FlameCallback>)callback;
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
