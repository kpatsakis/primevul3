static NetworkTechnology ParseNetworkTechnology(
    const std::string& technology) {
    if (technology == kNetworkTechnology1Xrtt)
    return NETWORK_TECHNOLOGY_1XRTT;
  if (technology == kNetworkTechnologyEvdo)
    return NETWORK_TECHNOLOGY_EVDO;
  if (technology == kNetworkTechnologyGprs)
    return NETWORK_TECHNOLOGY_GPRS;
  if (technology == kNetworkTechnologyEdge)
    return NETWORK_TECHNOLOGY_EDGE;
  if (technology == kNetworkTechnologyUmts)
    return NETWORK_TECHNOLOGY_UMTS;
  if (technology == kNetworkTechnologyHspa)
    return NETWORK_TECHNOLOGY_HSPA;
  if (technology == kNetworkTechnologyHspaPlus)
    return NETWORK_TECHNOLOGY_HSPA_PLUS;
  if (technology == kNetworkTechnologyLte)
    return NETWORK_TECHNOLOGY_LTE;
  if (technology == kNetworkTechnologyLteAdvanced)
    return NETWORK_TECHNOLOGY_LTE_ADVANCED;
  return NETWORK_TECHNOLOGY_UNKNOWN;
}
