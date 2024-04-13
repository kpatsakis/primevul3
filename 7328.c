static void WriteAttributeToMemory(std::vector<unsigned char> *out,
                                   const char *name, const char *type,
                                   const unsigned char *data, int len) {
  out->insert(out->end(), name, name + strlen(name) + 1);
  out->insert(out->end(), type, type + strlen(type) + 1);

  int outLen = len;
  tinyexr::swap4(reinterpret_cast<unsigned int *>(&outLen));
  out->insert(out->end(), reinterpret_cast<unsigned char *>(&outLen),
              reinterpret_cast<unsigned char *>(&outLen) + sizeof(int));
  out->insert(out->end(), data, data + len);
}