bool IsDrag(const POINT& origin, const POINT& current) {
  return views::View::ExceededDragThreshold(current.x - origin.x,
                                            current.y - origin.y);
}
