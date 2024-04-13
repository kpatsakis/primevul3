void GpuProcessHostUIShim::OnAcceleratedSurfaceSuspend(int32 surface_id) {
  TRACE_EVENT0("renderer",
      "GpuProcessHostUIShim::OnAcceleratedSurfaceSuspend");

  RenderWidgetHostViewPort* view =
      GetRenderWidgetHostViewFromSurfaceID(surface_id);
  if (!view)
    return;

  view->AcceleratedSurfaceSuspend();
}
