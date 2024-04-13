void ChromotingInstance::TrapKey(uint32 usb_keycode, bool trap) {
  key_mapper_.TrapKey(usb_keycode, trap);
}
