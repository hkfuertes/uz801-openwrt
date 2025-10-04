include $(TOPDIR)/rules.mk

PKG_NAME:=msm8916-advanced-reboot
PKG_VERSION:=1.0
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define Package/msm8916-advanced-reboot
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Advanced reboot utility for EDL/Fastboot modes
endef

define Package/msm8916-advanced-reboot/description
  Utility to reboot device into special modes (EDL, bootloader, recovery)
endef

define Build/Compile
	$(TARGET_CC) $(TARGET_CFLAGS) -static -o $(PKG_BUILD_DIR)/msm8916-advanced-reboot \
		./src/msm8916-advanced-reboot.c
endef

define Package/msm8916-advanced-reboot/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/msm8916-advanced-reboot $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,msm8916-advanced-reboot))
