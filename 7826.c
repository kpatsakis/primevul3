bool ShapeRefiner::IsUpdatedShapesOrTypes(
    InferenceContext* c, const std::vector<ShapeAndType>& existing,
    const std::vector<ShapeAndType>& updated) {
  if (existing.size() != updated.size()) {
    return true;
  }
  for (int i = 0; i < existing.size(); i++) {
    if (!SameDefinedShape(c, existing[i].shape, updated[i].shape) ||
        existing[i].dtype != updated[i].dtype) {
      return true;
    }
  }
  return false;
}