void GLES2DecoderImpl::DoGetVertexAttribiv(
    GLuint index, GLenum pname, GLint* params) {
  VertexAttribManager::VertexAttribInfo* info =
      vertex_attrib_manager_->GetVertexAttribInfo(index);
  if (!info) {
    SetGLError(GL_INVALID_VALUE, "glGetVertexAttribiv: index out of range");
    return;
  }
  switch (pname) {
    case GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING: {
        BufferManager::BufferInfo* buffer = info->buffer();
        if (buffer && !buffer->IsDeleted()) {
          GLuint client_id;
          buffer_manager()->GetClientId(buffer->service_id(), &client_id);
          *params = client_id;
        }
        break;
      }
    case GL_VERTEX_ATTRIB_ARRAY_ENABLED:
      *params = info->enabled();
      break;
    case GL_VERTEX_ATTRIB_ARRAY_SIZE:
      *params = info->size();
      break;
    case GL_VERTEX_ATTRIB_ARRAY_STRIDE:
      *params = info->gl_stride();
      break;
    case GL_VERTEX_ATTRIB_ARRAY_TYPE:
      *params = info->type();
      break;
    case GL_VERTEX_ATTRIB_ARRAY_NORMALIZED:
      *params = static_cast<GLint>(info->normalized());
      break;
    case GL_VERTEX_ATTRIB_ARRAY_DIVISOR_ANGLE:
      *params = info->divisor();
      break;
    case GL_CURRENT_VERTEX_ATTRIB:
      params[0] = static_cast<GLint>(info->value().v[0]);
      params[1] = static_cast<GLint>(info->value().v[1]);
      params[2] = static_cast<GLint>(info->value().v[2]);
      params[3] = static_cast<GLint>(info->value().v[3]);
      break;
    default:
      NOTREACHED();
      break;
  }
}
