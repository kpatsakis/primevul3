void GpuProcessHostUIShim::SimulateHang() {
  Send(new GpuMsg_Hang());
}
