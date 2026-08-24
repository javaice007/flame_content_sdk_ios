#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/// 播放会话状态（仅描述"呈现"，不描述播放进度/播放器内部状态）。
typedef NS_ENUM(NSInteger, FlamePlayerSessionState) {
    FlamePlayerSessionStateRequested = 0,
    FlamePlayerSessionStatePreparing,
    FlamePlayerSessionStatePresented,
    FlamePlayerSessionStateDismissed,
    FlamePlayerSessionStateFailed,
};

/// 一次播放呈现的不可变上下文（内部；0.3.4 `12_PLAYER_RUNTIME_DESIGN.md` §2.1）。
/// sessionId 仅用于诊断（ps-<单调序列>），不是 componentId，也不承载权益。
@interface FlamePlayerSession : NSObject

@property (nonatomic, copy, readonly) NSString *sessionId;
@property (nonatomic, copy, readonly) NSString *ownerComponentId;
@property (nonatomic, readonly) NSInteger playletId;
@property (nonatomic, readonly) NSInteger episode;
/// 内部可写：仅 FlamePlayerRuntime 驱动流转。
@property (nonatomic, assign) FlamePlayerSessionState state;

+ (instancetype)new NS_UNAVAILABLE;
- (instancetype)init NS_UNAVAILABLE;

- (nullable instancetype)initWithSessionId:(NSString *)sessionId
                            ownerComponentId:(NSString *)ownerComponentId
                                   playletId:(NSInteger)playletId
                                     episode:(NSInteger)episode;

@end

NS_ASSUME_NONNULL_END
