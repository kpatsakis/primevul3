static void usage(void)
{
    printf(EXEC_NAME " Copyright (C) 2008-2010, 2012, 2013, 2017, 2019"
           "Matt Davis (enferex)\n"
           "Special thanks to all contributors and the 757 crew.\n"
           "This program comes with ABSOLUTELY NO WARRANTY\n"
           "This is free software, and you are welcome to redistribute it\n"
           "under certain conditions.  For details see the file 'LICENSE'\n"
           "that came with this software or visit:\n"
           "<http://www.gnu.org/licenses/gpl-3.0.txt>\n\n");
    
    printf("-- " EXEC_NAME " v" VER" --\n"
           "Usage: ./" EXEC_NAME " <file.pdf> [-i] [-w] [-q] [-s]\n"
           "\t -i Display PDF creator information\n"
           "\t -w Write the PDF versions and summary to disk\n"
           "\t -q Display only the number of versions contained in the PDF\n"
           "\t -s Scrub the previous history data from the specified PDF\n");

    exit(0);
}
