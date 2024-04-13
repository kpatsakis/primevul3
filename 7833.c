bool ShapeRefiner::SameDefinedShape(InferenceContext* c, ShapeHandle s0,
                                    ShapeHandle s1) {
  if (s0.SameHandle(s1)) {
    return true;
  }
  if (c->Rank(s0) != c->Rank(s1)) {
    return false;
  }
  if (!c->RankKnown(s0) && !c->RankKnown(s1)) {
    return false;
  }
  for (int i = 0; i < c->Rank(s0); ++i) {
    if (!c->Dim(s0, i).SameHandle(c->Dim(s1, i))) {
      int64 val0 = c->Value(c->Dim(s0, i));
      int64 val1 = c->Value(c->Dim(s1, i));
      if (val0 < 0 || val1 < 0 || val0 != val1) {
        return false;
      }
    }
  }

  return true;
}