void PaintPatcher::DerefPatch() {
  DCHECK(begin_paint_.is_patched());
  DCHECK(end_paint_.is_patched());
  --refcount_;
  if (refcount_ == 0) {
    begin_paint_.Unpatch();
    end_paint_.Unpatch();
  }
}
