WebKit::WebGeolocationService* RenderView::geolocationService() {
  if (!geolocation_dispatcher_.get())
    geolocation_dispatcher_.reset(new GeolocationDispatcher(this));
  return geolocation_dispatcher_.get();
}
