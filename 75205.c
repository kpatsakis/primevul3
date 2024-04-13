void BrowserTitlebar::Init() {
  GtkWidget* container_hbox = gtk_hbox_new(FALSE, 0);

  container_ = gtk_event_box_new();
  gtk_widget_set_name(container_, "chrome-browser-titlebar");
  gtk_event_box_set_visible_window(GTK_EVENT_BOX(container_), FALSE);
  gtk_container_add(GTK_CONTAINER(container_), container_hbox);

  g_signal_connect(G_OBJECT(container_), "scroll-event",
                   G_CALLBACK(OnScroll), this);

  g_signal_connect(window_, "window-state-event",
                   G_CALLBACK(OnWindowStateChanged), this);

  if (browser_window_->browser()->profile()->IsOffTheRecord() &&
      browser_window_->browser()->type() == Browser::TYPE_NORMAL) {
    GtkWidget* spy_guy = gtk_image_new_from_pixbuf(GetOTRAvatar());
    gtk_misc_set_alignment(GTK_MISC(spy_guy), 0.0, 1.0);
    GtkWidget* spy_frame = gtk_alignment_new(0.0, 0.0, 1.0, 1.0);
    gtk_alignment_set_padding(GTK_ALIGNMENT(spy_frame), kOTRMaximizedTopSpacing,
        kOTRBottomSpacing, kOTRSideSpacing, kOTRSideSpacing);
    gtk_widget_set_size_request(spy_guy, -1, 0);
    gtk_container_add(GTK_CONTAINER(spy_frame), spy_guy);
    gtk_box_pack_start(GTK_BOX(container_hbox), spy_frame, FALSE, FALSE, 0);
  }

  titlebar_alignment_ = gtk_alignment_new(0.0, 0.0, 1.0, 1.0);
  if (browser_window_->browser()->type() == Browser::TYPE_NORMAL) {
    gtk_box_pack_start(GTK_BOX(container_hbox), titlebar_alignment_, TRUE,
                       TRUE, 0);

    gtk_container_add(GTK_CONTAINER(titlebar_alignment_),
                      browser_window_->tabstrip()->widget());
  } else {
    gtk_box_pack_start(GTK_BOX(container_hbox), titlebar_alignment_, TRUE,
                       TRUE, 0);
    GtkWidget* app_mode_hbox = gtk_hbox_new(FALSE, kIconTitleSpacing);
    gtk_container_add(GTK_CONTAINER(titlebar_alignment_), app_mode_hbox);

    gtk_box_pack_start(GTK_BOX(app_mode_hbox),
        browser_window_->tabstrip()->widget(), FALSE, FALSE, 0);

    GtkWidget* favicon_event_box = gtk_event_box_new();
    gtk_event_box_set_visible_window(GTK_EVENT_BOX(favicon_event_box), FALSE);
    g_signal_connect(favicon_event_box, "button-press-event",
                     G_CALLBACK(OnButtonPressed), this);
    gtk_box_pack_start(GTK_BOX(app_mode_hbox), favicon_event_box, FALSE,
                       FALSE, 0);
    ResourceBundle& rb = ResourceBundle::GetSharedInstance();
    app_mode_favicon_ = gtk_image_new_from_pixbuf(
        rb.GetRTLEnabledPixbufNamed(IDR_PRODUCT_LOGO_16));
    g_object_set_data(G_OBJECT(app_mode_favicon_), "left-align-popup",
                      reinterpret_cast<void*>(true));
    gtk_container_add(GTK_CONTAINER(favicon_event_box), app_mode_favicon_);

    app_mode_title_ = gtk_label_new(NULL);
    gtk_label_set_ellipsize(GTK_LABEL(app_mode_title_), PANGO_ELLIPSIZE_END);
    gtk_misc_set_alignment(GTK_MISC(app_mode_title_), 0.0, 0.5);
    gtk_box_pack_start(GTK_BOX(app_mode_hbox), app_mode_title_, TRUE, TRUE,
                       0);

    theme_provider_ = GtkThemeProvider::GetFrom(
        browser_window_->browser()->profile());
    registrar_.Add(this, NotificationType::BROWSER_THEME_CHANGED,
                   NotificationService::AllSources());
    theme_provider_->InitThemesFor(this);
    UpdateTitleAndIcon();
  }

  titlebar_buttons_box_ = gtk_vbox_new(FALSE, 0);
  GtkWidget* buttons_hbox = gtk_hbox_new(FALSE, kButtonSpacing);
  top_padding_ = gtk_fixed_new();
  gtk_widget_set_size_request(top_padding_, -1, kButtonOuterPadding);
  gtk_box_pack_start(GTK_BOX(titlebar_buttons_box_), top_padding_, FALSE, FALSE,
                     0);
  gtk_box_pack_start(GTK_BOX(titlebar_buttons_box_), buttons_hbox, FALSE,
                     FALSE, 0);

  if (CommandLine::ForCurrentProcess()->HasSwitch("glen")) {
    close_button_.reset(BuildTitlebarButton(IDR_GLEN, IDR_GLEN, IDR_GLEN,
                                            buttons_hbox, IDS_GLEN));
  } else {
    close_button_.reset(BuildTitlebarButton(IDR_CLOSE, IDR_CLOSE_P, IDR_CLOSE_H,
                                            buttons_hbox,
                                            IDS_XPFRAME_CLOSE_TOOLTIP));
  }

  restore_button_.reset(BuildTitlebarButton(IDR_RESTORE, IDR_RESTORE_P,
                        IDR_RESTORE_H, buttons_hbox,
                        IDS_XPFRAME_RESTORE_TOOLTIP));
  maximize_button_.reset(BuildTitlebarButton(IDR_MAXIMIZE, IDR_MAXIMIZE_P,
                         IDR_MAXIMIZE_H, buttons_hbox,
                         IDS_XPFRAME_MAXIMIZE_TOOLTIP));
  minimize_button_.reset(BuildTitlebarButton(IDR_MINIMIZE, IDR_MINIMIZE_P,
                         IDR_MINIMIZE_H, buttons_hbox,
                         IDS_XPFRAME_MINIMIZE_TOOLTIP));

  gtk_widget_size_request(close_button_->widget(), &close_button_req_);
  gtk_widget_size_request(minimize_button_->widget(), &minimize_button_req_);
  gtk_widget_size_request(restore_button_->widget(), &restore_button_req_);

  gtk_box_pack_end(GTK_BOX(container_hbox), titlebar_buttons_box_, FALSE,
                   FALSE, 0);

  gtk_widget_show_all(container_);

  ActiveWindowWatcherX::AddObserver(this);
}
