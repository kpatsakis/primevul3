bool TypedUrlModelAssociator::IsAssociated(const std::string& typed_url) {
  DCHECK(expected_loop_ == MessageLoop::current());
  return id_map_.find(typed_url) != id_map_.end();
}
