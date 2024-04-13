void ChromotingInstance::SendTrappedKey(uint32 usb_keycode, bool pressed) {
  scoped_ptr<base::DictionaryValue> data(new base::DictionaryValue());
  data->SetInteger("usbKeycode", usb_keycode);
  data->SetBoolean("pressed", pressed);
  PostChromotingMessage("trappedKeyEvent", data.Pass());
}
