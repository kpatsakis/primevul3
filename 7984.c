  string TraceString(const OpKernelContext& ctx, bool verbose) const override {
    string op = profiler::TraceMeOp(name_view(), type_string_view());
    string equation = strings::StrCat("(", equation_, ")");
    if (verbose) {
      string shape = ShapeTraceString(ctx);
      if (!shape.empty()) {
        return profiler::TraceMeEncode(
            std::move(op), {{"equation", equation}, {"shape", shape}});
      }
    }
    return profiler::TraceMeEncode(std::move(op), {{"equation", equation}});
  }