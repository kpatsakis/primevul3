LoginUtils* LoginUtils::Get() {
  return Singleton<LoginUtilsWrapper>::get()->get();
}
