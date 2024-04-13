static NetworkRoamingState ParseRoamingState(
    const std::string& roaming_state) {
    if (roaming_state == kRoamingStateHome)
    return ROAMING_STATE_HOME;
  if (roaming_state == kRoamingStateRoaming)
    return ROAMING_STATE_ROAMING;
  if (roaming_state == kRoamingStateUnknown)
    return ROAMING_STATE_UNKNOWN;
  return ROAMING_STATE_UNKNOWN;
}
