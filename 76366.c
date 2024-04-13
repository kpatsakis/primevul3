void GLES2DecoderImpl::DoBindRenderbuffer(GLenum target, GLuint client_id) {
  RenderbufferManager::RenderbufferInfo* info = NULL;
  GLuint service_id = 0;
  if (client_id != 0) {
    info = GetRenderbufferInfo(client_id);
    if (!info) {
      glGenRenderbuffersEXT(1, &service_id);
      CreateRenderbufferInfo(client_id, service_id);
      info = GetRenderbufferInfo(client_id);
      IdAllocator* id_allocator =
          group_->GetIdAllocator(id_namespaces::kRenderbuffers);
      id_allocator->MarkAsUsed(client_id);
    } else {
      service_id = info->service_id();
    }
  }
  bound_renderbuffer_ = info;
  glBindRenderbufferEXT(target, service_id);
}
