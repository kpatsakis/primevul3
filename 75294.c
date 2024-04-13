int GetDPI(const PrintMsg_Print_Params* print_params) {
#if defined(OS_MACOSX)
  return printing::kPointsPerInch;
#else
  return static_cast<int>(print_params->dpi);
#endif  // defined(OS_MACOSX)
}
