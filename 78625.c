ColorChooserDialog::ColorChooserDialog(views::ColorChooserListener* listener,
                                       SkColor initial_color,
                                       gfx::NativeWindow owning_window)
    : listener_(listener) {
  DCHECK(listener_);
  CopyCustomColors(g_custom_colors, custom_colors_);
  ExecuteOpenParams execute_params(initial_color, BeginRun(owning_window),
                                   owning_window);
  execute_params.run_state.dialog_thread->message_loop()->PostTask(FROM_HERE,
      base::Bind(&ColorChooserDialog::ExecuteOpen, this, execute_params));
}
