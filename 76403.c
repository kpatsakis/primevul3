void GLES2DecoderImpl::DoUniform2fv(
    GLint location, GLsizei count, const GLfloat* value) {
  GLenum type;
  if (!GetUniformTypeByLocation(location, "glUniform2fv", &type)) {
    return;
  }
  if (type == GL_BOOL_VEC2) {
    GLsizei num_values = count * 2;
    scoped_array<GLint> temp(new GLint[num_values]);
    for (GLsizei ii = 0; ii < num_values; ++ii) {
      temp[ii] = static_cast<GLint>(value[ii]);
    }
    glUniform2iv(location, count, temp.get());
  } else {
    glUniform2fv(location, count, value);
  }
}
