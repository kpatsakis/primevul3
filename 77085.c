void DesktopNativeWidgetHelperAura::PostInitialize() {
#if defined(OS_WIN)
  subclass_.reset(new ui::HWNDSubclass(root_window_->GetAcceleratedWidget()));
  subclass_->SetFilter(new WidgetMessageFilter(root_window_.get(),
                                               widget_->GetWidget()));
#endif
}
