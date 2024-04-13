GdkAtom StringToGdkAtom(const std::string& str) {
  return gdk_atom_intern(str.c_str(), FALSE);
}
