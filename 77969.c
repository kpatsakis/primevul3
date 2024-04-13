void RunCloseWithAppMenuCallback(Browser* browser) {
  base::MessageLoop::current()->PostTask(
      FROM_HERE, base::Bind(&CloseWindowCallback, browser));
  chrome::ShowAppMenu(browser);
}
