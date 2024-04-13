Status ShapeRefiner::UpdateNode(const Node* node, bool relax, bool* refined) {
  auto it = node_to_context_.find(node);
  if (it == node_to_context_.end()) {
    *refined = true;
    return AddNode(node);
  }
  ExtendedInferenceContext* node_ext_context = it->second.get();
  InferenceContext* node_context = node_ext_context->get_context();

  // Give up if the context wasn't successfully built by the AddNode() method.
  TF_RETURN_IF_ERROR(node_context->construction_status());

  // Check if the shapes of the nodes in the fan-in of this node have changed,
  // and if they have update the node input shapes.
  for (const Edge* e : node->in_edges()) {
    if (e->IsControlEdge()) continue;

    int dst_input = e->dst_input();
    int src_output = e->src_output();

    Node* input = e->src();
    auto iter = node_to_context_.find(input);
    if (iter == node_to_context_.end()) {
      return errors::FailedPrecondition(
          "Input ", dst_input, " ('", input->name(), "') for '", node->name(),
          "' was not previously added to ShapeRefiner.");
    }

    InferenceContext* c = iter->second->get_context();
    DCHECK_GE(dst_input, 0);
    ShapeHandle existing_input = node_context->input(dst_input);
    if (!relax) {
      if (node_context->MergeInput(dst_input, c->output(src_output))) {
        if (!SameDefinedShape(node_context, node_context->input(dst_input),
                              existing_input)) {
          *refined = true;
        }
      }
    } else {
      if (node_context->RelaxInput(dst_input, c->output(src_output))) {
        if (!SameDefinedShape(node_context, node_context->input(dst_input),
                              existing_input)) {
          *refined = true;
        }
      }
    }
    if (node_context->requested_input_tensor_as_partial_shape(dst_input)) {
      // The input value may have changed. Since we have no way to know if
      // that's indeed the case, err on the safe side.
      *refined = true;
    }

    // Also propagate handle shape and dtype of edges which are carrying
    // resource handles.
    if (e->src()->output_type(src_output) == DT_RESOURCE) {
      auto* outputs = c->output_handle_shapes_and_types(src_output);
      if (!outputs) continue;

      if (!relax &&
          node_context->MergeInputHandleShapesAndTypes(dst_input, *outputs)) {
        *refined = true;
      } else if (relax) {
        std::vector<ShapeAndType> existing_inputs;
        const std::vector<ShapeAndType>* inputs =
            node_context->input_handle_shapes_and_types(dst_input);
        if (inputs) {
          existing_inputs = *inputs;
        }
        if (node_context->RelaxInputHandleShapesAndMergeTypes(dst_input,
                                                              *outputs)) {
          if (IsUpdatedShapesOrTypes(
                  node_context, existing_inputs,
                  *node_context->input_handle_shapes_and_types(dst_input))) {
            *refined = true;
          }
        }
      }
    }
  }

  if (!*refined) {
    // No input shape has changed, we're done
    return Status::OK();
  }

  // Get and run the shape function for this node to update the shapes of the
  // outputs.
  const OpRegistrationData* op_reg_data;
  TF_RETURN_IF_ERROR(ops_registry_->LookUp(node->type_string(), &op_reg_data));
  if (op_reg_data->shape_inference_fn == nullptr &&
      require_shape_inference_fns_) {
    return errors::InvalidArgument(
        "No shape inference function exists for op '", node->type_string(),
        "', did you forget to define it?");
  }

  if (!op_reg_data->shape_inference_fn) {
    // There is nothing more we can infer
    return Status::OK();
  }

  return RunShapeFn(node, op_reg_data, node_ext_context);
}