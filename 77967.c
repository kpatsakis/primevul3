void OnZoomLevelChanged(const base::Closure& callback,
                        const HostZoomMap::ZoomLevelChange& host) {
  callback.Run();
}
