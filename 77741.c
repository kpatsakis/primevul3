void InputMethodBase::DispatchFabricatedKeyEventPostIME(EventType type,
                                                        KeyboardCode key_code,
                                                        int flags) const {
  if (delegate_)
    delegate_->DispatchFabricatedKeyEventPostIME(type, key_code, flags);
}
