  bool ApplyConfig() {
    DCHECK(context_->ui_task_runner()->BelongsToCurrentThread());

    if (!config_.GetString(kHostIdConfigPath, &host_id_)) {
      LOG(ERROR) << "host_id is not defined in the config.";
      return false;
    }

    if (!key_pair_.Load(config_)) {
      return false;
    }

    std::string host_secret_hash_string;
    if (!config_.GetString(kHostSecretHashConfigPath,
                           &host_secret_hash_string)) {
      host_secret_hash_string = "plain:";
    }

    if (!host_secret_hash_.Parse(host_secret_hash_string)) {
      LOG(ERROR) << "Invalid host_secret_hash.";
      return false;
    }

    if (!config_.GetString(kXmppLoginConfigPath, &xmpp_login_) ||
        !(config_.GetString(kXmppAuthTokenConfigPath, &xmpp_auth_token_) ||
          config_.GetString(kOAuthRefreshTokenConfigPath,
                            &oauth_refresh_token_))) {
      LOG(ERROR) << "XMPP credentials are not defined in the config.";
      return false;
    }

    config_.GetBoolean(kOAuthUseOfficialClientIdConfigPath,
                       &oauth_use_official_client_id_);

    if (!oauth_refresh_token_.empty()) {
      xmpp_auth_token_ = "";  // This will be set to the access token later.
      xmpp_auth_service_ = "oauth2";
    } else if (!config_.GetString(kXmppAuthServiceConfigPath,
                                  &xmpp_auth_service_)) {
      xmpp_auth_service_ = kChromotingTokenDefaultServiceName;
    }
    return true;
  }
