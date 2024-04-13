Status ShapeRefiner::InferShapesForFunction(
    const FunctionDef* function_def, AttrSlice attributes,
    ExtendedInferenceContext* outer_context) {
  const Graph* graph;
  auto it = functions_.find(function_def);
  if (it != functions_.end()) {
    graph = it->second.get();
  } else {
    InstantiationResult result;
    TF_RETURN_IF_ERROR(InstantiateFunction(
        *function_def, attributes,
        [this](const string& op, const OpDef** sig) {
          return this->function_library_->LookUpOpDef(op, sig);
        },
        &result));

    Graph* new_graph = new Graph(function_library_);
    GraphConstructorOptions options;
    options.allow_internal_ops = true;
    TF_RETURN_IF_ERROR(
        ConvertNodeDefsToGraph(options, result.nodes, new_graph));
    functions_[function_def].reset(new_graph);
    graph = new_graph;
  }

  std::unordered_set<const Node*> function_nodes;
  Status inference_status = Status::OK();
  {
    auto node_shape_inference_lambda = [this, &outer_context, &function_nodes,
                                        &inference_status](const Node* node) {
      if (!inference_status.ok()) return;
      inference_status =
          InferShapesForFunctionSubNode(node, outer_context->get_context());
      function_nodes.insert(node);
    };

    // Calls inference lambda for each node after visiting all predecessors.
    // Ensures that we are adding nodes to ShapeRefiner in the topological
    // order.
    ReverseDFS(*graph, {}, node_shape_inference_lambda);
  }

  // Delete the contexts created for the functions nodes to save memory.
  for (const Node* node : function_nodes) {
    node_to_context_.erase(node);
  }

  return inference_status;
}