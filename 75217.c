void BrowserTitlebar::ShowContextMenu() {
  if (!context_menu_.get()) {
    static const MenuCreateMaterial context_menu_blueprint[] = {
        { MENU_NORMAL, IDC_NEW_TAB, IDS_TAB_CXMENU_NEWTAB, 0, NULL,
            GDK_t, GDK_CONTROL_MASK, true },
        { MENU_NORMAL, IDC_RESTORE_TAB, IDS_RESTORE_TAB, 0, NULL,
            GDK_t, GDK_CONTROL_MASK | GDK_SHIFT_MASK, true },
        { MENU_SEPARATOR },
        { MENU_NORMAL, IDC_TASK_MANAGER, IDS_TASK_MANAGER, 0, NULL,
            GDK_Escape, GDK_SHIFT_MASK, true },
#if !defined(OS_CHROMEOS)
        { MENU_SEPARATOR },
        { MENU_CHECKBOX, kShowWindowDecorationsCommand,
            IDS_SHOW_WINDOW_DECORATIONS_MENU },
#endif
        { MENU_END },
    };

    context_menu_.reset(new MenuGtk(this, context_menu_blueprint, NULL));
  }

  context_menu_->PopupAsContext(gtk_get_current_event_time());
}
