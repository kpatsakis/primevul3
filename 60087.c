router_dir_info_changed(void)
{
  need_to_update_have_min_dir_info = 1;
  rend_hsdir_routers_changed();
}
