GdkPixbuf* GetOTRAvatar() {
  static GdkPixbuf* otr_avatar = NULL;
  if (!otr_avatar) {
    ResourceBundle& rb = ResourceBundle::GetSharedInstance();
    otr_avatar = rb.GetRTLEnabledPixbufNamed(IDR_OTR_ICON);
  }
  return otr_avatar;
}
