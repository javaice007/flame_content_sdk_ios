Pod::Spec.new do |s|
  s.name             = 'flame_content_sdk_ios'
  s.version          = '0.2.0'
  s.summary          = 'Flame Content SDK for iOS.'
  s.description      = 'Flame Content SDK: content capability plus Direct GroMore rewarded ads behind a single Flame-facing API.'
  s.homepage         = 'https://github.com/javaice007/flame_content_sdk_ios'
  s.license          = { :type => 'Proprietary', :text => 'Distribution terms are defined by Flame.' }
  s.author           = { 'Flame' => 'sdk@invalid.local' }
  s.source           = { :git => 'https://github.com/javaice007/flame_content_sdk_ios.git', :branch => 'pre-release/0.2.0' }
  s.ios.deployment_target = '13.0'
  s.requires_arc     = true
  s.module_name      = 'flame_content_sdk_ios'
  s.frameworks       = ['Foundation', 'UIKit']
  s.vendored_frameworks = 'flame_content_sdk_ios.xcframework'
  s.pod_target_xcconfig = {
    'OTHER_LDFLAGS' => ['$(inherited)', '-undefined', 'dynamic_lookup']
  }
  s.default_subspecs = ['Content']

  s.subspec 'CoreAds' do |ads|
    ads.dependency 'Ads-CN/CSJMediation-Only', '7.7.0.6'
  end

  s.subspec 'Content' do |csj|
    csj.dependency 'flame_content_sdk_ios/CoreAds'
    csj.dependency 'PangrowthX/shortplay', '2.9.0.6'
    csj.dependency 'TTSDKFramework/Player-SR', '1.42.3.4-premium'
  end
end
