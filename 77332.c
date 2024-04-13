void PaintPatcher::RefPatch() {
  if (refcount_ == 0) {
    DCHECK(!begin_paint_.is_patched());
    DCHECK(!end_paint_.is_patched());
    begin_paint_.Patch(L"riched20.dll", "user32.dll", "BeginPaint",
                       &BeginPaintIntercept);
    end_paint_.Patch(L"riched20.dll", "user32.dll", "EndPaint",
                     &EndPaintIntercept);
  }
  ++refcount_;
}
