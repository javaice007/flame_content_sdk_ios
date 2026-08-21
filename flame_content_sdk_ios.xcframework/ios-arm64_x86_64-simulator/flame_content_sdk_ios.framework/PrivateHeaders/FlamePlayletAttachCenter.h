#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/// 抽象「可挂接的短剧配置」能力面（不经任何 DJX 类型；设备侧由 DJXPlayletConfig category 适配）。
/// 抽象值：unlockMode 2 = Specific（对应 DJXPlayletUnlockADMode_Specific）。
@protocol FlamePlayletAttachable <NSObject>
@property (nonatomic, assign) NSInteger flame_unlockMode;
@property (nonatomic, assign) NSInteger flame_freeEpisodesCount;
@property (nonatomic, assign) NSInteger flame_unlockEpisodesCount;
@property (nonatomic, weak, nullable) id flame_unlockInterfaceDelegate;
@end

/// 短剧配置挂接中心（A4B；纯 Foundation，模拟器可测）。
/// 职责：对每一个进入 Specific Unlock 的 DJXPlayletConfig 实例应用挂接策略：
///   playletUnlockADMode = Specific + interfaceDelegate = 解锁适配器（单例，抵消 weak）。
/// counts（free/unlock）来自配置创建方（官方默认/后端），本中心只透传不发明。
/// 官方 28261：滑滑流进入新详情页实例时原 delegate 失效 → 提供「详情页开始/入口点击」
/// 重挂通道（noteDetailPageStarted / noteEnterDetailRequested），对最近挂接的活跃配置重放策略。
@interface FlamePlayletAttachCenter : NSObject

+ (instancetype)sharedCenter;

/// 解锁 delegate 提供者 seam（设备默认 = FlameCSJCustomUnlockAdapter 单例；测试注入 Fake）。
@property (nonatomic, copy, nullable) id _Nullable (^unlockDelegateProvider)(void);

/// 挂接（幂等：同一实例重复挂接不重复计数；重复调用会重放 delegate——弱引用被置空后可恢复）。
- (void)attachPlayletConfig:(id<FlamePlayletAttachable>)config;

/// 官方要求的详情页重挂（滑滑流 → 新详情页实例）：对最近活跃的已挂接配置重放策略。
- (void)noteDetailPageStarted;
/// 用户从混排流点击「进入详情页」（DJXPlayletInterfaceProtocol.clickEnterView 时机）。
- (void)noteEnterDetailRequested;

/// 当前仍存活的已挂接配置数（诊断/测试；NSMapTable 弱键，配置释放自动移除）。
- (NSUInteger)attachedConfigCount;

/// 最近一次挂接的活跃配置（弱引用；测试观测）。
@property (nonatomic, readonly, nullable) id<FlamePlayletAttachable> lastAttachedConfig;

- (void)resetForTesting;

@end

NS_ASSUME_NONNULL_END
