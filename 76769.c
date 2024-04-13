void RootWindow::RemoveRootWindowObserver(RootWindowObserver* observer) {
  observers_.RemoveObserver(observer);
}
