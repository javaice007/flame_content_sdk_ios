#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/// 内部 eCPM 信息（A2 不暴露 Public API；A3 Custom Unlock Bridge 与后续上报使用）。
/// 单位约定：rawValue 为 GroMore 原始字符串，官方口径为「分」（CNY 口径，见 A1 04 文档）。
typedef NS_ENUM(NSInteger, FlameEcpmAvailability) {
    FlameEcpmAvailabilityUnavailable = 0,  // nil / 空串 / 长度为 0
    FlameEcpmAvailabilityNoPermission,     // GroMore 无权限值 "-3"（展示无权限口径）
    FlameEcpmAvailabilityZeroValue,        // "0"
    FlameEcpmAvailabilityAvailable,        // 正常数值字符串
};

@interface FlameEcpmInfo : NSObject

@property (nonatomic, copy, readonly, nullable) NSString *rawValue;   // 原始 ecpm 字符串（分）
@property (nonatomic, assign, readonly) FlameEcpmAvailability availability;
@property (nonatomic, copy, readonly, nullable) NSString *adnName;
@property (nonatomic, copy, readonly, nullable) NSString *slotID;
@property (nonatomic, copy, readonly, nullable) NSString *levelTag;
@property (nonatomic, copy, readonly, nullable) NSString *biddingTypeName;

- (instancetype)initWithRawValue:(nullable NSString *)rawValue
                         adnName:(nullable NSString *)adnName
                          slotID:(nullable NSString *)slotID
                         levelTag:(nullable NSString *)levelTag
                biddingTypeName:(nullable NSString *)biddingTypeName;

+ (instancetype)unavailableInfo;

/// 兼容旧触发路径：nil/空 → Unavailable。
+ (FlameEcpmAvailability)classifyRawValue:(nullable NSString *)rawValue;

@end

NS_ASSUME_NONNULL_END
