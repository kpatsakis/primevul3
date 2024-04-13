  WarmingObserver() {
    NetworkLibrary *netlib = CrosLibrary::Get()->GetNetworkLibrary();
    netlib->AddNetworkManagerObserver(this);
  }
