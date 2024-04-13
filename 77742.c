void InputMethodBase::DispatchKeyEventPostIME(
    const base::NativeEvent& native_event) const {
  if (delegate_)
    delegate_->DispatchKeyEventPostIME(native_event);
}
