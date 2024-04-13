PluginInfoBarDelegate::PluginInfoBarDelegate(TabContents* tab_contents,
                                             const string16& name)
    : ConfirmInfoBarDelegate(tab_contents),
      name_(name),
      tab_contents_(tab_contents) {
}
