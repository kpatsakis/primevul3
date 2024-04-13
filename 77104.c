void GpuProcessHostUIShim::SimulateCrash() {
  Send(new GpuMsg_Crash());
}
