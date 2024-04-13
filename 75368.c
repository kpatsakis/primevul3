PluginObserver::PluginObserver(TabContentsWrapper* tab_contents)
    : TabContentsObserver(tab_contents->tab_contents()),
      tab_contents_(tab_contents) {
}
