  void RevokeScheme(const std::string& scheme) {
    scheme_policy_[scheme] = false;
  }
