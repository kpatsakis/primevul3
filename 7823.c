ShapeRefiner::~ShapeRefiner() {
  // The lifetime of the tensors are bound to the GraphRunner, so the tensors
  // should be deleted before it.
  const_tensor_map_.clear();
}