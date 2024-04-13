void LoginUtils::Set(LoginUtils* mock) {
  Singleton<LoginUtilsWrapper>::get()->reset(mock);
}
