static void display_creator(FILE *fp, const pdf_t *pdf)
{
    int i;

    printf("PDF Version: %d.%d\n",
           pdf->pdf_major_version, pdf->pdf_minor_version);

    for (i=0; i<pdf->n_xrefs; ++i)
    {
        if (!pdf->xrefs[i].version)
          continue;
      
        if (pdf_display_creator(pdf, i))
          printf("\n");
    }
}
