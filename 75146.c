  ~LangToExemplarSet() {
    STLDeleteContainerPairSecondPointers(map.begin(), map.end());
  }
