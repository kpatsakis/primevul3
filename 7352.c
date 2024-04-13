static void hufClearDecTable(HufDec *hdecod)  // io: (allocated by caller)
//     decoding table [HUF_DECSIZE]
{
  for (int i = 0; i < HUF_DECSIZE; i++) {
    hdecod[i].len = 0;
    hdecod[i].lit = 0;
    hdecod[i].p = NULL;
  }
  // memset(hdecod, 0, sizeof(HufDec) * HUF_DECSIZE);
}