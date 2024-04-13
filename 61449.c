static void MagickUsage(MagickBooleanType verbose)
{
  const char
    *name;

  size_t
    len;

  name=GetClientName();
  len=strlen(name);

  if (len>=7 && LocaleCompare("convert",name+len-7) == 0) {
    /* convert usage */
    (void) FormatLocaleFile(stdout,
       "Usage: %s [ {option} | {image} ... ] {output_image}\n",name);
    (void) FormatLocaleFile(stdout,
       "       %s -help | -version | -usage | -list {option}\n\n",name);
    return;
  }
  else if (len>=6 && LocaleCompare("script",name+len-6) == 0) {
    /* magick-script usage */
    (void) FormatLocaleFile(stdout,
      "Usage: %s {filename} [ {script_args} ... ]\n",name);
  }
  else {
    /* magick usage */
    (void) FormatLocaleFile(stdout,
       "Usage: %s [ {option} | {image} ... ] {output_image}\n",name);
    (void) FormatLocaleFile(stdout,
       "       %s [ {option} | {image} ... ] -script {filename} [ {script_args} ...]\n",
       name);
  }
  (void) FormatLocaleFile(stdout,
    "       %s -help | -version | -usage | -list {option}\n\n",name);

  if (verbose == MagickFalse)
    return;

  (void) FormatLocaleFile(stdout,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s\n",
    "All options are performed in a strict 'as you see them' order\n",
    "You must read-in images before you can operate on them.\n",
    "\n",
    "Magick Script files can use any of the following forms...\n",
    "     #!/path/to/magick -script\n",
    "or\n",
    "     #!/bin/sh\n",
    "     :; exec magick -script \"$0\" \"$@\"; exit 10\n",
    "     # Magick script from here...\n",
    "or\n",
    "     #!/usr/bin/env  magick-script\n",
    "The latter two forms do not require the path to the command hard coded.\n",
    "Note: \"magick-script\" needs to be linked to the \"magick\" command.\n",
    "\n",
    "For more information on usage, options, examples, and techniques\n",
    "see the ImageMagick website at    ", MagickAuthoritativeURL);

  return;
}
