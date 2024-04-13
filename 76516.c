static bool CheckParameters(AudioParameters params) {
  if (!params.IsValid())
    return false;
  return true;
}
