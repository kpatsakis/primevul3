void RootWindow::SetGestureRecognizerForTesting(ui::GestureRecognizer* gr) {
  gesture_recognizer_.reset(gr);
}
