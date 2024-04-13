static void scrub_document(FILE *fp, const pdf_t *pdf)
{
    FILE *new_fp;
    int   ch, i, j, last_version ;
    char *new_name, *c;
    const char *suffix = "-scrubbed.pdf";

    /* Create a new name */
    if (!(new_name = malloc(strlen(pdf->name) + strlen(suffix) + 1)))
    {
        ERR("Insufficient memory to create scrubbed file name\n");
        return;
    }

    strcpy(new_name, pdf->name);
    if ((c = strrchr(new_name, '.')))
      *c = '\0';
    strcat(new_name, suffix);

    if ((new_fp = fopen(new_name, "r")))
    {
        ERR("File name already exists for saving scrubbed document\n");
        free(new_name);
        fclose(new_fp);
        return;
    }

    if (!(new_fp = fopen(new_name, "w+")))
    {
        ERR("Could not create file for saving scrubbed document\n");
        free(new_name);
        fclose(new_fp);
        return;
    }

    /* Copy */
    fseek(fp, SEEK_SET, 0);
    while ((ch = fgetc(fp)) != EOF)
      fputc(ch, new_fp); 

    /* Find last version (dont zero these baddies) */
    last_version = 0;
    for (i=0; i<pdf->n_xrefs; i++)
      if (pdf->xrefs[i].version)
        last_version = pdf->xrefs[i].version;
   
    /* Zero mod objects from all but the most recent version
     * Zero del objects from all versions
     */
    fseek(new_fp, 0, SEEK_SET);
    for (i=0; i<pdf->n_xrefs; i++)
    {
        for (j=0; j<pdf->xrefs[i].n_entries; j++)
          if (!pdf->xrefs[i].entries[j].obj_id)
            continue;
          else
          {
              switch (pdf_get_object_status(pdf, i, j))
              {
                  case 'M':
                      if (pdf->xrefs[i].version != last_version)
                        pdf_zero_object(new_fp, pdf, i, j);
                      break;

                  case 'D':
                      pdf_zero_object(new_fp, pdf, i, j);
                      break;

                  default:
                      break;
              }
          }
    }

    /* Clean */
    free(new_name);
    fclose(new_fp);
}
