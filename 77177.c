static GraphicsContext3D::SourceDataFormat getSourceDataFormat(unsigned int componentsPerPixel, AlphaFormat alphaFormat, bool is16BitFormat, bool bigEndian)
{
    const static SourceDataFormatBase formatTableBase[4][AlphaFormatNumFormats] = { // componentsPerPixel x AlphaFormat
        { SourceFormatBaseR,          SourceFormatBaseA,          SourceFormatBaseA          }, // 1 componentsPerPixel
        { SourceFormatBaseNumFormats, SourceFormatBaseAR,         SourceFormatBaseRA         }, // 2 componentsPerPixel
        { SourceFormatBaseRGB,        SourceFormatBaseNumFormats, SourceFormatBaseNumFormats }, // 3 componentsPerPixel
        { SourceFormatBaseNumFormats, SourceFormatBaseARGB,       SourceFormatBaseRGBA        } // 4 componentsPerPixel
    };
    const static GraphicsContext3D::SourceDataFormat formatTable[SourceFormatBaseNumFormats][4] = { // SourceDataFormatBase x bitsPerComponent x endian
        { GraphicsContext3D::SourceFormatR8,    GraphicsContext3D::SourceFormatR8,    GraphicsContext3D::SourceFormatR16Little,    GraphicsContext3D::SourceFormatR16Big },
        { GraphicsContext3D::SourceFormatA8,    GraphicsContext3D::SourceFormatA8,    GraphicsContext3D::SourceFormatA16Little,    GraphicsContext3D::SourceFormatA16Big },
        { GraphicsContext3D::SourceFormatAR8,   GraphicsContext3D::SourceFormatRA8,   GraphicsContext3D::SourceFormatRA16Little,   GraphicsContext3D::SourceFormatRA16Big },
        { GraphicsContext3D::SourceFormatRA8,   GraphicsContext3D::SourceFormatAR8,   GraphicsContext3D::SourceFormatAR16Little,   GraphicsContext3D::SourceFormatAR16Big },
        { GraphicsContext3D::SourceFormatBGR8,  GraphicsContext3D::SourceFormatRGB8,  GraphicsContext3D::SourceFormatRGB16Little,  GraphicsContext3D::SourceFormatRGB16Big },
        { GraphicsContext3D::SourceFormatABGR8, GraphicsContext3D::SourceFormatRGBA8, GraphicsContext3D::SourceFormatRGBA16Little, GraphicsContext3D::SourceFormatRGBA16Big },
        { GraphicsContext3D::SourceFormatBGRA8, GraphicsContext3D::SourceFormatARGB8, GraphicsContext3D::SourceFormatARGB16Little, GraphicsContext3D::SourceFormatARGB16Big }
    };

    ASSERT(componentsPerPixel <= 4 && componentsPerPixel > 0);
    SourceDataFormatBase formatBase = formatTableBase[componentsPerPixel - 1][alphaFormat];
    if (formatBase == SourceFormatBaseNumFormats)
        return GraphicsContext3D::SourceFormatNumFormats;
    return formatTable[formatBase][(is16BitFormat ? 2 : 0) + (bigEndian ? 1 : 0)];
}
