static bool ReadAttribute(std::string *name, std::string *type,
                          std::vector<unsigned char> *data, size_t *marker_size,
                          const char *marker, size_t size) {
  size_t name_len = strnlen(marker, size);
  if (name_len == size) {
    // String does not have a terminating character.
    return false;
  }
  *name = std::string(marker, name_len);

  marker += name_len + 1;
  size -= name_len + 1;

  size_t type_len = strnlen(marker, size);
  if (type_len == size) {
    return false;
  }
  *type = std::string(marker, type_len);

  marker += type_len + 1;
  size -= type_len + 1;

  if (size < sizeof(uint32_t)) {
    return false;
  }

  uint32_t data_len;
  memcpy(&data_len, marker, sizeof(uint32_t));
  tinyexr::swap4(reinterpret_cast<unsigned int *>(&data_len));

  if (data_len == 0) {
    if ((*type).compare("string") == 0) {
      // Accept empty string attribute.

      marker += sizeof(uint32_t);
      size -= sizeof(uint32_t);

      *marker_size = name_len + 1 + type_len + 1 + sizeof(uint32_t);

      data->resize(1);
      (*data)[0] = '\0';

      return true;
    } else {
      return false;
    }
  }

  marker += sizeof(uint32_t);
  size -= sizeof(uint32_t);

  if (size < data_len) {
    return false;
  }

  data->resize(static_cast<size_t>(data_len));
  memcpy(&data->at(0), marker, static_cast<size_t>(data_len));

  *marker_size = name_len + 1 + type_len + 1 + sizeof(uint32_t) + data_len;
  return true;
}