OmniboxViewWin::~OmniboxViewWin() {
  text_object_model_->Release();

  g_paint_patcher.Pointer()->DerefPatch();
}
