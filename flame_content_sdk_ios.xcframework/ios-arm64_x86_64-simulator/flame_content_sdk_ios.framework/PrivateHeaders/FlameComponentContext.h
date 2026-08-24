#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/// 组件类型（内部；与公开 FlameComponentState 分离）。
typedef NS_ENUM(NSInteger, FlameComponentKind) {
    FlameComponentKindFeed = 0,
    FlameComponentKindPlayer,
    FlameComponentKindTheater,
    FlameComponentKindFeedStream,
};

/// 每组件一份的不可变身份对象（Runtime Core 设计 §2.2）。
/// 只保存"身份 + 创建参数快照 + 诊断 ID"；不保存权益、Login、Vendor 对象或可变运行状态。
@interface FlameComponentContext : NSObject

@property (nonatomic, copy, readonly) NSString *componentId;
@property (nonatomic, readonly) FlameComponentKind kind;
@property (nonatomic, copy, readonly) NSDictionary<NSString *, id> *optionsSnapshot;
@property (nonatomic, copy, readonly) NSString *diagnosticTraceId;

+ (instancetype)new NS_UNAVAILABLE;
- (instancetype)init NS_UNAVAILABLE;

/// 仅供 SDK 内部构造。componentId 由 Runtime 主线程生成（fc-<前缀>-<单调序列>，
/// 永不复用，不派生自内容 ID / 顺序 / Vendor 标识）。
- (nullable instancetype)initWithComponentId:(NSString *)componentId
                                        kind:(FlameComponentKind)kind
                                     options:(nullable NSDictionary<NSString *, id> *)options;

@end

NS_ASSUME_NONNULL_END
