void LoginUtils::DoBrowserLaunch(Profile* profile) {
  BootTimesLoader::Get()->AddLoginTimeMarker("BrowserLaunched", false);
  if (!LoginUtils::Get()->IsBrowserLaunchEnabled())
    return;

  CommandLine::ForCurrentProcess()->InitFromArgv(
      CommandLine::ForCurrentProcess()->argv());

  VLOG(1) << "Launching browser...";
  BrowserInit browser_init;
  int return_code;
  browser_init.LaunchBrowser(*CommandLine::ForCurrentProcess(),
                             profile,
                             FilePath(),
                             true,
                             &return_code);
}
