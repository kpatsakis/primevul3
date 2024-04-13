void RootWindow::PostMouseMoveEventAfterWindowChange() {
  if (synthesize_mouse_move_)
    return;
  synthesize_mouse_move_ = true;
  MessageLoop::current()->PostTask(
      FROM_HERE,
      base::Bind(&RootWindow::SynthesizeMouseMoveEvent,
                 event_factory_.GetWeakPtr()));
}
