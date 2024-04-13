static ActivationState ParseActivationState(
    const std::string& activation_state) {
  if (activation_state == kActivationStateActivated)
    return ACTIVATION_STATE_ACTIVATED;
  if (activation_state == kActivationStateActivating)
    return ACTIVATION_STATE_ACTIVATING;
  if (activation_state == kActivationStateNotActivated)
    return ACTIVATION_STATE_NOT_ACTIVATED;
  if (activation_state == kActivationStateUnknown)
    return ACTIVATION_STATE_UNKNOWN;
  if (activation_state == kActivationStatePartiallyActivated)
    return ACTIVATION_STATE_PARTIALLY_ACTIVATED;
  return ACTIVATION_STATE_UNKNOWN;
}
