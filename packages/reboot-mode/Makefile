include $(TOPDIR)/rules.mk

PKG_NAME:=reboot-mode
PKG_VERSION:=1.0
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define Package/reboot-mode
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Reboot mode utility for Qualcomm devices
  DEPENDS:=@(TARGET_msm89xx)
  KCONFIG:=CONFIG_DEVMEM=y
endef

define Build/Compile
	$(TARGET_CC) $(TARGET_CFLAGS) $(TARGET_LDFLAGS) \
		-o $(PKG_BUILD_DIR)/reboot-mode \
		./src/reboot-mode.c
endef

define Package/reboot-mode/install
	$(INSTALL_DIR) $(1)/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/reboot-mode $(1)/sbin/
endef

$(eval $(call BuildPackage,reboot-mode))
