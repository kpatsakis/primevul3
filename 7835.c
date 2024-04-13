Status ShapeRefiner::AddNode(const Node* node) {
  return AddNodeInternal(node, /*outer_context=*/nullptr);
}