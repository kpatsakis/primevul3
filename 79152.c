DevToolsWindow* DevToolsWindow::CreateDevToolsWindowForWorker(
    Profile* profile) {
  content::RecordAction(content::UserMetricsAction("DevTools_InspectWorker"));
  return Create(profile, GURL(), NULL, DEVTOOLS_DOCK_SIDE_UNDOCKED, true,
                false);
}
