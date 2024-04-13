void RemoveDuplicateElements(
    std::vector<string16>* elements, std::vector<int>* unique_ids) {
  std::vector<string16> copy;
  for (size_t i = 0; i < elements->size(); ++i) {
    const string16& element = (*elements)[i];

    bool unique = true;
    for (std::vector<string16>::const_iterator copy_iter = copy.begin();
         copy_iter != copy.end(); ++copy_iter) {
      if (element == *copy_iter) {
        unique = false;
        break;
      }
    }

    if (unique)
      copy.push_back(element);
    else
      unique_ids->erase(unique_ids->begin() + i);
  }

  elements->assign(copy.begin(), copy.end());
}
