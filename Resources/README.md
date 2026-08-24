# Resources

The XCFramework does not embed customer configuration or customer data.

Flame delivers `FlameCustomerConfig.bundle` separately for the target App and Bundle ID. The Host adds
the intact bundle to its App target's Copy Bundle Resources. Do not add `sdk_setting_file.json`,
`live-creds.json`, `.env` files, test fixtures, or Vendor temporary files to this binary package.
