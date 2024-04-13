void MenuGtk::OnMenuShow(GtkWidget* widget, MenuGtk* menu) {
  MessageLoop::current()->PostTask(FROM_HERE,
      menu->factory_.NewRunnableMethod(&MenuGtk::UpdateMenu));
}
