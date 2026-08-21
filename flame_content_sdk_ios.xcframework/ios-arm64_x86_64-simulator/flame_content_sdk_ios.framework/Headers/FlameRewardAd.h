#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@protocol FlameRewardListener <NSObject>
@optional
- (void)onAdLoaded;
- (void)onAdShow;
- (void)onAdClicked;
- (void)onAdPlayComplete;
- (void)onAdReward:(NSString *)userId userCustomData:(NSString *)userCustomData transId:(NSString *)transId;
- (void)onAdClosed;
- (void)onAdError:(NSString *)code desc:(NSString *)desc;
@end

@protocol FlameRewardAd <NSObject>
- (void)load;
- (void)loadWithUserId:(NSString *)userId userCustomData:(NSString *)userCustomData;
- (BOOL)isReady;
- (BOOL)isLoading;
- (void)show;
- (void)destroy;
@end

NS_ASSUME_NONNULL_END
