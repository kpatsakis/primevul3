should_apply_guardfraction(const networkstatus_t *ns)
{
  /* We need to check the corresponding torrc option and the consensus
   * parameter if we need to. */
  const or_options_t *options = get_options();

  /* If UseGuardFraction is 'auto' then check the same-named consensus
   * parameter. If the consensus parameter is not present, default to
   * "off". */
  if (options->UseGuardFraction == -1) {
    return networkstatus_get_param(ns, "UseGuardFraction",
                                   0, /* default to "off" */
                                   0, 1);
  }

  return options->UseGuardFraction;
}
