cmsBool ReadOneWChar(cmsIOHANDLER* io,  _cmsDICelem* e, cmsUInt32Number i, wchar_t ** wcstr)
{

    cmsUInt32Number nChars;

      if (e -> Offsets[i] == 0) {

          *wcstr = NULL;
          return TRUE;
      }

      if (!io -> Seek(io, e -> Offsets[i])) return FALSE;

      nChars = e ->Sizes[i] / sizeof(cmsUInt16Number);


      *wcstr = (wchar_t*) _cmsMallocZero(e ->ContextID, (nChars + 1) * sizeof(wchar_t));
      if (*wcstr == NULL) return FALSE;

      if (!_cmsReadWCharArray(io, nChars, *wcstr)) {
          _cmsFree(e ->ContextID, *wcstr);
          return FALSE;
      }

      (*wcstr)[nChars] = 0;
      return TRUE;
}
