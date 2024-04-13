void RootWindowHostWin::OnClose() {
  MessageLoopForUI::current()->Quit();
}
