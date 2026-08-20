#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/// Flame Content SDK 统一字符串错误码。
/// 分段规则（详见 docs/context/接入GroMore广告/12_A2_ERROR_AND_THREADING.md）：
/// - 14000/14002/14003/14004：沿用旧 Flame SDK 公共语义（未初始化/参数错误/无可用 Provider（含不支持的平台码）/非法广告位）
/// - 1410x：配置与初始化链路
/// - 1411x：广告对象生命周期
/// 三方原始错误码不直接进入本命名空间，统一映射后附带在 desc 中。
FOUNDATION_EXPORT NSString * const FlameContentErrorNotInitialized;        // 14000
FOUNDATION_EXPORT NSString * const FlameContentErrorInvalidParameter;      // 14002
FOUNDATION_EXPORT NSString * const FlameContentErrorUnsupportedProvider;   // 14003
FOUNDATION_EXPORT NSString * const FlameContentErrorInvalidPlacement;      // 14004

FOUNDATION_EXPORT NSString * const FlameContentErrorNetwork;               // 14101 网络请求失败
FOUNDATION_EXPORT NSString * const FlameContentErrorResponseMalformed;     // 14102 响应格式非法
FOUNDATION_EXPORT NSString * const FlameContentErrorDecryptFailed;         // 14103 edata 解密失败
FOUNDATION_EXPORT NSString * const FlameContentErrorConfigIncomplete;      // 14104 配置缺关键字段
FOUNDATION_EXPORT NSString * const FlameContentErrorProviderInitFailed;    // 14105 广告平台初始化失败

FOUNDATION_EXPORT NSString * const FlameContentErrorRewardLoadFailed;      // 14110 加载失败
FOUNDATION_EXPORT NSString * const FlameContentErrorRewardNotReady;        // 14111 未就绪调用 show
FOUNDATION_EXPORT NSString * const FlameContentErrorRewardShowFailed;      // 14112 展示失败
FOUNDATION_EXPORT NSString * const FlameContentErrorRewardVerifyFailed;    // 14113 奖励验证失败
FOUNDATION_EXPORT NSString * const FlameContentErrorInvalidState;          // 14114 非法状态（destroyed 等）

FOUNDATION_EXPORT NSString * const FlameContentErrorContentConfigMissing;     // 14115 内容配置文件缺失
FOUNDATION_EXPORT NSString * const FlameContentErrorContentConfigInvalid;      // 14116 内容配置文件非法
FOUNDATION_EXPORT NSString * const FlameContentErrorContentInitFailed;         // 14117 内容初始化失败
FOUNDATION_EXPORT NSString * const FlameContentErrorContentStartFailed;        // 14118 内容启动失败
FOUNDATION_EXPORT NSString * const FlameContentErrorContentResourceMissing;    // 14119 内容资源缺失
FOUNDATION_EXPORT NSString * const FlameContentErrorContentUnavailable;        // 14120 内容能力不可用（平台/形态限制）
FOUNDATION_EXPORT NSString * const FlameContentErrorContentUnsupportedProvider; // 14121 不支持的内容 Provider
FOUNDATION_EXPORT NSString * const FlameContentErrorContentInvalidState;       // 14122 内容状态非法

/// - 1413x：自定义解锁桥（A4A；docs/context/自定义解锁/）
FOUNDATION_EXPORT NSString * const FlameContentErrorUnlockSceneMissing;         // 14130 content_unlock 场景缺失
FOUNDATION_EXPORT NSString * const FlameContentErrorUnlockPlacementMissing;    // 14131 Flame Placement 无 pIds 映射
FOUNDATION_EXPORT NSString * const FlameContentErrorUnlockAdCreateFailed;      // 14132 内部 Reward 创建失败
FOUNDATION_EXPORT NSString * const FlameContentErrorUnlockPresenterMissing;    // 14133 无可展示页面
FOUNDATION_EXPORT NSString * const FlameContentErrorUnlockRewardVerifyFailed;  // 14134 奖励验证失败（verify==NO）
FOUNDATION_EXPORT NSString * const FlameContentErrorUnlockSessionInvalid;      // 14135 会话无效/过期
FOUNDATION_EXPORT NSString * const FlameContentErrorUnlockCpmUnavailable;      // 14136 eCPM 不可用（诊断；仍按官方口径回传空串）

FOUNDATION_EXPORT NSString * const FlameContentErrorSkeletonNotImplemented; // 19999（保留）

/// /sdk/init 中标识 Direct GroMore 的平台码（后端冻结契约）。
FOUNDATION_EXPORT NSString * const FlameProviderCodeGroMore;  // "gm"

NS_ASSUME_NONNULL_END
