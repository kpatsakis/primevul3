error::Error GLES2DecoderImpl::HandleDrawElements(
    uint32 immediate_data_size, const gles2::DrawElements& c) {
  return DoDrawElements(false,
                        static_cast<GLenum>(c.mode),
                        static_cast<GLsizei>(c.count),
                        static_cast<GLenum>(c.type),
                        static_cast<int32>(c.index_offset),
                        0);
}
