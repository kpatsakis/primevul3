void BeginInstallWithManifestFunction::SetResult(ResultCode code) {
  switch (code) {
    case ERROR_NONE:
      result_.reset(Value::CreateStringValue(""));
      break;
    case UNKNOWN_ERROR:
      result_.reset(Value::CreateStringValue("unknown_error"));
      break;
    case USER_CANCELLED:
      result_.reset(Value::CreateStringValue("user_cancelled"));
      break;
    case MANIFEST_ERROR:
      result_.reset(Value::CreateStringValue("manifest_error"));
      break;
    case ICON_ERROR:
      result_.reset(Value::CreateStringValue("icon_error"));
      break;
    case INVALID_ID:
      result_.reset(Value::CreateStringValue("invalid_id"));
      break;
    case PERMISSION_DENIED:
      result_.reset(Value::CreateStringValue("permission_denied"));
      break;
    case NO_GESTURE:
      result_.reset(Value::CreateStringValue("no_gesture"));
      break;
    default:
      CHECK(false);
   }
 }
