void RootWindow::AdvanceQueuedTouchEvent(Window* window, bool processed) {
  scoped_ptr<ui::GestureRecognizer::Gestures> gestures;
  gestures.reset(gesture_recognizer_->AdvanceTouchQueue(window, processed));
  ProcessGestures(gestures.get());
}
