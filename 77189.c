std::string GdkAtomToString(const GdkAtom& atom) {
  gchar* name = gdk_atom_name(atom);
  std::string rv(name);
  g_free(name);
  return rv;
}
