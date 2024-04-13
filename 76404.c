void GLES2DecoderImpl::DoUniform3fv(
    GLint location, GLsizei count, const GLfloat* value) {
  GLenum type;
  if (!GetUniformTypeByLocation(location, "glUniform3fv", &type)) {
    return;
  }
  if (type == GL_BOOL_VEC3) {
    GLsizei num_values = count * 3;
    scoped_array<GLint> temp(new GLint[num_values]);
    for (GLsizei ii = 0; ii < num_values; ++ii) {
      temp[ii] = static_cast<GLint>(value[ii]);
    }
    glUniform3iv(location, count, temp.get());
  } else {
    glUniform3fv(location, count, value);
  }
}
