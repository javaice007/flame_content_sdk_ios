#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface FlameContentAppEntity : NSObject
@property (nonatomic, copy, nullable) NSString *pt;
@property (nonatomic, copy, nullable) NSString *aId;
@property (nonatomic, copy, nullable) NSString *aKey;
@property (nonatomic, copy) NSDictionary<NSString *, NSString *> *pIds;
@property (nonatomic, copy) NSDictionary<NSString *, NSString *> *scenePIds;
@property (nonatomic, copy, nullable) NSString *contentProvider;

+ (nullable instancetype)entityFromDictionary:(NSDictionary *)dictionary;
- (nullable NSString *)flamePlacementIdForScene:(NSString *)scene;
- (nullable NSString *)externalPlacementIdForFlamePlacementId:(NSString *)placementId;

/// 直接消费 /sdk/init 解密后的 JSON 数据（前向兼容：未知字段忽略）。
+ (nullable instancetype)entityFromDecryptedJSONData:(NSData *)data;

/// 当前配置是否具备初始化广告平台的最低要素（pt 与 aId 非空）。
- (BOOL)isViableForAdsProvider;

@end

NS_ASSUME_NONNULL_END
