void GLES2DecoderImpl::DoUniform4fv(
    GLint location, GLsizei count, const GLfloat* value) {
  GLenum type;
  if (!GetUniformTypeByLocation(location, "glUniform4fv", &type)) {
    return;
  }
  if (type == GL_BOOL_VEC4) {
    GLsizei num_values = count * 4;
    scoped_array<GLint> temp(new GLint[num_values]);
    for (GLsizei ii = 0; ii < num_values; ++ii) {
      temp[ii] = static_cast<GLint>(value[ii]);
    }
    glUniform4iv(location, count, temp.get());
  } else {
    glUniform4fv(location, count, value);
  }
}
