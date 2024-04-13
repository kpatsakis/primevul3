Status ShapeRefiner::SetShape(const Node* node, int output_port,
                              ShapeHandle shape) {
  auto c = GetContext(node);
  if (c == nullptr) {
    return errors::Internal("Could not find context for ", node->name());
  }

  if (output_port < 0 || output_port >= node->num_outputs()) {
    return errors::InvalidArgument(
        "output_port '", output_port, "' is out of range, ", "node '",
        node->name(), "' has ", node->num_outputs(), " outputs");
  }
  // Note: it's possible, if the node's been updated, that the shape inference
  // context doesn't have the right number of outputs.
  if (node->num_outputs() > c->num_outputs()) {
    TF_RETURN_IF_ERROR(c->ExpandOutputs(node->num_outputs()));
  }

  // Check compatibility, and merge the shapes.
  ShapeHandle existing_shape = c->output(output_port);
  TF_RETURN_IF_ERROR(c->Merge(existing_shape, shape, &shape));
  c->set_output(output_port, shape);

  // TODO(vrv): Do we need to propagate the new shape through all
  // consumers that change their outputs?  At the moment, python
  // does not do this, but this seems like a nice feature.

  // TODO(vrv): We might need to keep track of the fact that the
  // existing shape is invalidated, in case we need to propagate
  // this information to remote workers.
  return Status::OK();
}