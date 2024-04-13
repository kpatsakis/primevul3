error::Error GLES2DecoderImpl::HandlePixelStorei(
    uint32 immediate_data_size, const gles2::PixelStorei& c) {
  GLenum pname = c.pname;
  GLenum param = c.param;
  if (!validators_->pixel_store.IsValid(pname)) {
    SetGLError(GL_INVALID_ENUM, "glPixelStorei: pname GL_INVALID_ENUM");
    return error::kNoError;
  }
  switch (pname) {
    case GL_PACK_ALIGNMENT:
    case GL_UNPACK_ALIGNMENT:
        if (!validators_->pixel_store_alignment.IsValid(param)) {
            SetGLError(GL_INVALID_VALUE,
                       "glPixelSTore: param GL_INVALID_VALUE");
            return error::kNoError;
        }
    default:
        break;
  }
  glPixelStorei(pname, param);
  switch (pname) {
  case GL_PACK_ALIGNMENT:
      pack_alignment_ = param;
      break;
  case GL_PACK_REVERSE_ROW_ORDER_ANGLE:
      break;
  case GL_UNPACK_ALIGNMENT:
      unpack_alignment_ = param;
      break;
  default:
      NOTREACHED();
      break;
  }
  return error::kNoError;
}
