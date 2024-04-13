static void hufFreeDecTable(HufDec *hdecod)  // io: Decoding table
{
  for (int i = 0; i < HUF_DECSIZE; i++) {
    if (hdecod[i].p) {
      delete[] hdecod[i].p;
      hdecod[i].p = 0;
    }
  }
}