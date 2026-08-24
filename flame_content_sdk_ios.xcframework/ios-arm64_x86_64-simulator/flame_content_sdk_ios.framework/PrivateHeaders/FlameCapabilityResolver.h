#import <Foundation/Foundation.h>

@class FlameComponentRuntime;

NS_ASSUME_NONNULL_BEGIN

/// 能力协商（Runtime Core 设计 §2.6）。
/// 只认 Adapter 声明的能力位，不按 Vendor 名称或版本号推断支持。
/// 能力缺失时 factory fail closed（返回 nil），不降级为猜测路由。
@interface FlameCapabilityResolver : NSObject

- (instancetype)initWithRuntime:(FlameComponentRuntime *)runtime;
+ (instancetype)new NS_UNAVAILABLE;
- (instancetype)init NS_UNAVAILABLE;

/// Feed 组件创建能力：要求 Data Adapter 存在且 contentDataCapabilityAvailable。
/// playerAdapter 可选缺失（此时 Feed → Player 路由 fail closed），不阻塞 Feed 创建。
- (BOOL)feedComponentCapabilityAvailable:(NSString *_Nullable *_Nullable)missingCode
                                     desc:(NSString *_Nullable *_Nullable)missingDesc;

/// Theater 组件创建能力：要求 Data Adapter 存在且实现 Theater 能力组
/// （categoryCapabilityAvailable 为 YES；@optional 组未实现视为无能力，fail closed）。
- (BOOL)theaterComponentCapabilityAvailable:(NSString *_Nullable *_Nullable)missingCode
                                        desc:(NSString *_Nullable *_Nullable)missingDesc;

@end

NS_ASSUME_NONNULL_END
