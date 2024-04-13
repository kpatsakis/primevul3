void GpuProcessHostUIShim::SimulateRemoveAllContext() {
  Send(new GpuMsg_Clean());
}
