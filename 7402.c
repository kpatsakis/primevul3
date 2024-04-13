inline void sendCode(long long sCode, int runCount, long long runCode,
                     long long &c, int &lc, char *&out) {
  //
  // Output a run of runCount instances of the symbol sCount.
  // Output the symbols explicitly, or if that is shorter, output
  // the sCode symbol once followed by a runCode symbol and runCount
  // expressed as an 8-bit number.
  //

  if (hufLength(sCode) + hufLength(runCode) + 8 < hufLength(sCode) * runCount) {
    outputCode(sCode, c, lc, out);
    outputCode(runCode, c, lc, out);
    outputBits(8, runCount, c, lc, out);
  } else {
    while (runCount-- >= 0) outputCode(sCode, c, lc, out);
  }
}