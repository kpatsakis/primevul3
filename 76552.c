void ChromotingInstance::HandleMessage(const pp::Var& message) {
  if (!message.is_string()) {
    LOG(ERROR) << "Received a message that is not a string.";
    return;
  }

  scoped_ptr<base::Value> json(
      base::JSONReader::Read(message.AsString(),
                             base::JSON_ALLOW_TRAILING_COMMAS));
  base::DictionaryValue* message_dict = NULL;
  std::string method;
  base::DictionaryValue* data = NULL;
  if (!json.get() ||
      !json->GetAsDictionary(&message_dict) ||
      !message_dict->GetString("method", &method) ||
      !message_dict->GetDictionary("data", &data)) {
    LOG(ERROR) << "Received invalid message:" << message.AsString();
    return;
  }

  if (method == "connect") {
    ClientConfig config;
    std::string auth_methods;
    if (!data->GetString("hostJid", &config.host_jid) ||
        !data->GetString("hostPublicKey", &config.host_public_key) ||
        !data->GetString("localJid", &config.local_jid) ||
        !data->GetString("sharedSecret", &config.shared_secret) ||
        !data->GetString("authenticationMethods", &auth_methods) ||
        !ParseAuthMethods(auth_methods, &config) ||
        !data->GetString("authenticationTag", &config.authentication_tag)) {
      LOG(ERROR) << "Invalid connect() data.";
      return;
    }

    Connect(config);
  } else if (method == "disconnect") {
    Disconnect();
  } else if (method == "incomingIq") {
    std::string iq;
    if (!data->GetString("iq", &iq)) {
      LOG(ERROR) << "Invalid onIq() data.";
      return;
    }
    OnIncomingIq(iq);
  } else if (method == "releaseAllKeys") {
    ReleaseAllKeys();
  } else if (method == "injectKeyEvent") {
    int usb_keycode = 0;
    bool is_pressed = false;
    if (!data->GetInteger("usbKeycode", &usb_keycode) ||
        !data->GetBoolean("pressed", &is_pressed)) {
      LOG(ERROR) << "Invalid injectKeyEvent.";
      return;
    }

    protocol::KeyEvent event;
    event.set_usb_keycode(usb_keycode);
    event.set_pressed(is_pressed);
    InjectKeyEvent(event);
  } else if (method == "remapKey") {
    int from_keycode = 0;
    int to_keycode = 0;
    if (!data->GetInteger("fromKeycode", &from_keycode) ||
        !data->GetInteger("toKeycode", &to_keycode)) {
      LOG(ERROR) << "Invalid remapKey.";
      return;
    }

    RemapKey(from_keycode, to_keycode);
  } else if (method == "trapKey") {
    int keycode = 0;
    bool trap = false;
    if (!data->GetInteger("keycode", &keycode) ||
        !data->GetBoolean("trap", &trap)) {
      LOG(ERROR) << "Invalid trapKey.";
      return;
    }

    TrapKey(keycode, trap);
  } else if (method == "sendClipboardItem") {
    std::string mime_type;
    std::string item;
    if (!data->GetString("mimeType", &mime_type) ||
        !data->GetString("item", &item)) {
      LOG(ERROR) << "Invalid sendClipboardItem() data.";
      return;
    }
    SendClipboardItem(mime_type, item);
  } else if (method == "notifyClientDimensions") {
    int width = 0;
    int height = 0;
    if (!data->GetInteger("width", &width) ||
        !data->GetInteger("height", &height)) {
      LOG(ERROR) << "Invalid notifyClientDimensions.";
      return;
    }
    NotifyClientDimensions(width, height);
  } else if (method == "pauseVideo") {
    bool pause = false;
    if (!data->GetBoolean("pause", &pause)) {
      LOG(ERROR) << "Invalid pauseVideo.";
      return;
    }
    PauseVideo(pause);
  } else if (method == "pauseAudio") {
    bool pause = false;
    if (!data->GetBoolean("pause", &pause)) {
      LOG(ERROR) << "Invalid pauseAudio.";
      return;
    }
    PauseAudio(pause);
  }
}
