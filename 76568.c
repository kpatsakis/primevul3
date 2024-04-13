void ChromotingInstance::RemapKey(uint32 in_usb_keycode,
                                  uint32 out_usb_keycode) {
  key_mapper_.RemapKey(in_usb_keycode, out_usb_keycode);
}
