Status RaggedCountSparseOutputShapeFn(InferenceContext *c) {
  int32_t rank = c->Rank(c->input(1));
  if (rank != c->kUnknownRank) {
    ++rank;  // Add the ragged dimension
  }
  auto nvals = c->UnknownDim();
  c->set_output(0, c->Matrix(nvals, rank));  // out.indices
  c->set_output(1, c->Vector(nvals));        // out.values
  c->set_output(2, c->Vector(rank));         // out.dense_shape
  return Status::OK();
}