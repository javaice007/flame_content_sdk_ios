#import <Foundation/Foundation.h>
#import "FlameContentAppEntity.h"

NS_ASSUME_NONNULL_BEGIN

/// 配置完成回调（主线程）：entity 与 code/desc 互斥。
typedef void (^FlameConfigCompletion)(FlameContentAppEntity *_Nullable entity, NSString *_Nullable code, NSString *_Nullable desc);

/// Flame 配置管理：负责 POST /api/v1/sdk/init（MD5 签名 + edata AES-GCM 解密）→ FlameContentAppEntity。
///
/// 线程与并发：
/// - completion 保证主线程
/// - requestGeneration 代际去重：新请求/clear 后，旧请求的 completion 会被丢弃
/// - A2 不做磁盘缓存（避免复制旧 SDK aKey 明文落盘债务；每次 init 拉取远端配置）
@interface FlameContentConfigManager : NSObject

+ (instancetype)sharedManager;

@property (atomic, strong, nullable, readonly) FlameContentAppEntity *currentConfig;

/// 发起 /sdk/init。appId/appKey 为 Flame 平台凭证；sdkVersion 参与 sign。
- (void)updateAppWithAppId:(NSString *)appId
                    appKey:(NSString *)appKey
                sdkVersion:(NSString *)sdkVersion
                 completion:(FlameConfigCompletion)completion;

- (void)installConfig:(FlameContentAppEntity *)config;
- (void)clear;

- (nullable NSString *)externalPlacementIdForFlamePlacementId:(NSString *)placementId;
- (nullable NSString *)externalPlacementIdForScene:(NSString *)scene;

@end

NS_ASSUME_NONNULL_END
