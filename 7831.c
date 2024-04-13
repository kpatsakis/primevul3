Status ShapeRefiner::EvaluateConstantTensorForEdge(
    const Node* node, int dst_idx, bool* evaluated, Tensor* result,
    InferenceContext* outer_context) {
  *evaluated = false;
  const Edge* input_edge;
  TF_RETURN_IF_ERROR(node->input_edge(dst_idx, &input_edge));
  OutputTensor tensor(input_edge->src(), input_edge->src_output());
  return EvaluateConstantTensor(
      tensor, *this, *ops_registry_, graph_def_version_, evaluated, result,
      &graph_runner_, &const_tensor_map_, kMaxTensorSize,
      disable_constant_propagation_, outer_context);
}