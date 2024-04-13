bool ChromotingInstance::ParseAuthMethods(const std::string& auth_methods_str,
                                          ClientConfig* config) {
  std::vector<std::string> auth_methods;
  base::SplitString(auth_methods_str, ',', &auth_methods);
  for (std::vector<std::string>::iterator it = auth_methods.begin();
       it != auth_methods.end(); ++it) {
    protocol::AuthenticationMethod authentication_method =
        protocol::AuthenticationMethod::FromString(*it);
    if (authentication_method.is_valid())
      config->authentication_methods.push_back(authentication_method);
  }
  if (config->authentication_methods.empty()) {
    LOG(ERROR) << "No valid authentication methods specified.";
    return false;
  }

  return true;
}
