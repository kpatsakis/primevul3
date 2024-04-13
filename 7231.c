static void hufBuildEncTable(
    long long *frq,  // io: input frequencies [HUF_ENCSIZE], output table
    int *im,         //  o: min frq index
    int *iM)         //  o: max frq index
{
  //
  // This function assumes that when it is called, array frq
  // indicates the frequency of all possible symbols in the data
  // that are to be Huffman-encoded.  (frq[i] contains the number
  // of occurrences of symbol i in the data.)
  //
  // The loop below does three things:
  //
  // 1) Finds the minimum and maximum indices that point
  //    to non-zero entries in frq:
  //
  //     frq[im] != 0, and frq[i] == 0 for all i < im
  //     frq[iM] != 0, and frq[i] == 0 for all i > iM
  //
  // 2) Fills array fHeap with pointers to all non-zero
  //    entries in frq.
  //
  // 3) Initializes array hlink such that hlink[i] == i
  //    for all array entries.
  //

  std::vector<int> hlink(HUF_ENCSIZE);
  std::vector<long long *> fHeap(HUF_ENCSIZE);

  *im = 0;

  while (!frq[*im]) (*im)++;

  int nf = 0;

  for (int i = *im; i < HUF_ENCSIZE; i++) {
    hlink[i] = i;

    if (frq[i]) {
      fHeap[nf] = &frq[i];
      nf++;
      *iM = i;
    }
  }

  //
  // Add a pseudo-symbol, with a frequency count of 1, to frq;
  // adjust the fHeap and hlink array accordingly.  Function
  // hufEncode() uses the pseudo-symbol for run-length encoding.
  //

  (*iM)++;
  frq[*iM] = 1;
  fHeap[nf] = &frq[*iM];
  nf++;

  //
  // Build an array, scode, such that scode[i] contains the number
  // of bits assigned to symbol i.  Conceptually this is done by
  // constructing a tree whose leaves are the symbols with non-zero
  // frequency:
  //
  //     Make a heap that contains all symbols with a non-zero frequency,
  //     with the least frequent symbol on top.
  //
  //     Repeat until only one symbol is left on the heap:
  //
  //         Take the two least frequent symbols off the top of the heap.
  //         Create a new node that has first two nodes as children, and
  //         whose frequency is the sum of the frequencies of the first
  //         two nodes.  Put the new node back into the heap.
  //
  // The last node left on the heap is the root of the tree.  For each
  // leaf node, the distance between the root and the leaf is the length
  // of the code for the corresponding symbol.
  //
  // The loop below doesn't actually build the tree; instead we compute
  // the distances of the leaves from the root on the fly.  When a new
  // node is added to the heap, then that node's descendants are linked
  // into a single linear list that starts at the new node, and the code
  // lengths of the descendants (that is, their distance from the root
  // of the tree) are incremented by one.
  //

  std::make_heap(&fHeap[0], &fHeap[nf], FHeapCompare());

  std::vector<long long> scode(HUF_ENCSIZE);
  memset(scode.data(), 0, sizeof(long long) * HUF_ENCSIZE);

  while (nf > 1) {
    //
    // Find the indices, mm and m, of the two smallest non-zero frq
    // values in fHeap, add the smallest frq to the second-smallest
    // frq, and remove the smallest frq value from fHeap.
    //

    int mm = fHeap[0] - frq;
    std::pop_heap(&fHeap[0], &fHeap[nf], FHeapCompare());
    --nf;

    int m = fHeap[0] - frq;
    std::pop_heap(&fHeap[0], &fHeap[nf], FHeapCompare());

    frq[m] += frq[mm];
    std::push_heap(&fHeap[0], &fHeap[nf], FHeapCompare());

    //
    // The entries in scode are linked into lists with the
    // entries in hlink serving as "next" pointers and with
    // the end of a list marked by hlink[j] == j.
    //
    // Traverse the lists that start at scode[m] and scode[mm].
    // For each element visited, increment the length of the
    // corresponding code by one bit. (If we visit scode[j]
    // during the traversal, then the code for symbol j becomes
    // one bit longer.)
    //
    // Merge the lists that start at scode[m] and scode[mm]
    // into a single list that starts at scode[m].
    //

    //
    // Add a bit to all codes in the first list.
    //

    for (int j = m;; j = hlink[j]) {
      scode[j]++;

      assert(scode[j] <= 58);

      if (hlink[j] == j) {
        //
        // Merge the two lists.
        //

        hlink[j] = mm;
        break;
      }
    }

    //
    // Add a bit to all codes in the second list
    //

    for (int j = mm;; j = hlink[j]) {
      scode[j]++;

      assert(scode[j] <= 58);

      if (hlink[j] == j) break;
    }
  }

  //
  // Build a canonical Huffman code table, replacing the code
  // lengths in scode with (code, code length) pairs.  Copy the
  // code table from scode into frq.
  //

  hufCanonicalCodeTable(scode.data());
  memcpy(frq, scode.data(), sizeof(long long) * HUF_ENCSIZE);
}