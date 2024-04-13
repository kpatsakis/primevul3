void CheckXEventForConsistency(XEvent* xevent) {
#if defined(USE_XI2_MT) && !defined(NDEBUG)
  static bool expect_master_event = false;
  static XIDeviceEvent slave_event;
  static gfx::Point slave_location;
  static int slave_button;


  bool was_expecting_master_event = expect_master_event;
  expect_master_event = false;

  if (!xevent || xevent->type != GenericEvent)
    return;

  XIDeviceEvent* xievent = static_cast<XIDeviceEvent*>(xevent->xcookie.data);
  if (xievent->evtype != XI_Motion &&
      xievent->evtype != XI_ButtonPress &&
      xievent->evtype != XI_ButtonRelease) {
    return;
  }

  if (xievent->sourceid == xievent->deviceid) {
    slave_event = *xievent;
    slave_location = ui::EventLocationFromNative(xevent);
    slave_button = ui::EventButtonFromNative(xevent);
    expect_master_event = true;
  } else if (was_expecting_master_event) {
    CHECK_EQ(slave_location.x(), ui::EventLocationFromNative(xevent).x());
    CHECK_EQ(slave_location.y(), ui::EventLocationFromNative(xevent).y());

    CHECK_EQ(slave_event.type, xievent->type);
    CHECK_EQ(slave_event.evtype, xievent->evtype);
    CHECK_EQ(slave_button, ui::EventButtonFromNative(xevent));
    CHECK_EQ(slave_event.flags, xievent->flags);
    CHECK_EQ(slave_event.buttons.mask_len, xievent->buttons.mask_len);
    CHECK_EQ(slave_event.valuators.mask_len, xievent->valuators.mask_len);
    CHECK_EQ(slave_event.mods.base, xievent->mods.base);
    CHECK_EQ(slave_event.mods.latched, xievent->mods.latched);
    CHECK_EQ(slave_event.mods.locked, xievent->mods.locked);
    CHECK_EQ(slave_event.mods.effective, xievent->mods.effective);
  }
#endif  // defined(USE_XI2_MT) && !defined(NDEBUG)
}
