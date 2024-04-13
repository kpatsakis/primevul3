MenuGtk::~MenuGtk() {
  menu_.Destroy();
  STLDeleteContainerPointers(submenus_we_own_.begin(), submenus_we_own_.end());
  if (dummy_accel_group_)
    g_object_unref(dummy_accel_group_);
}
