#include "widget.h"

#include <hb.h>
#include <hb-ft.h>	// HarfBuzz/FreeType bridge
#include <ft2build.h>

#include <QPainter>
#include <QRawFont>
#include <QGlyphRun>

#define ARABIC_TEST
//#define THAI_TEST

#ifdef ARABIC_TEST
#ifdef _ANDROID_PLATFORM
static const char *fileName = "/sdcard/harfbuzztest/arial.ttf";
#endif // _ANDROID_PLATFORM

#ifdef _WINDOWS_PLATFORM
static const char *fileName = "C:/Windows/Fonts/arial.ttf";
#endif // _WINDOWS_PLATFORM

static const char *textString = "شلاؤيثبل";
#endif // ARABIC_TEST

#ifdef THAI_TEST
#ifdef _ANDROID_PLATFORM
static const char *fileName = "/sdcard/harfbuzztest/Garuda.ttf";
#endif // _ANDROID_PLATFORM

#ifdef _WINDOWS_PLATFORM
static const char *fileName = "G:/Qt/harfbuzztest/run/Garuda.ttf";
#endif // _WINDOWS_PLATFORM

static const char *textString = "ฟิแกำดเ้ร่าสทื";
#endif // THAI_TEST

static int fontSize = 50;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
}

Widget::~Widget()
{
}

void Widget::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);

    qreal marginSize = 10.0;
    qreal fontScale = 64.0;

    // prepare FreeType font
    FT_Library ftLibrary;
    FT_Init_FreeType(&ftLibrary);

    FT_Face ftFace;
    FT_New_Face(ftLibrary, fileName, 0, &ftFace);
    FT_Set_Pixel_Sizes(ftFace, 0, fontSize);

    // prepare HarfBuzz font and buffer
    hb_font_t *hbFont = hb_ft_font_create(ftFace, NULL);
    hb_buffer_t *hbBuffer = hb_buffer_create();

    // HarfBuzz buffers have single direction, script and language,
    // bidirectional multiscript text have to be itemized first
    //
    // for simple strings HarfBuzz can guess the properties
    //

#ifdef ARABIC_TEST
    hb_buffer_set_direction(hbBuffer, HB_DIRECTION_RTL);
    hb_buffer_set_script(hbBuffer, HB_SCRIPT_ARABIC);
    hb_buffer_set_language(hbBuffer, hb_language_from_string("ar",0));
#endif // ARABIC_TEST

#ifdef THAI_TEST
    hb_buffer_set_direction(hbBuffer, HB_DIRECTION_LTR);
    hb_buffer_set_script(hbBuffer, HB_SCRIPT_THAI);
    hb_buffer_set_language(hbBuffer, hb_language_from_string("th",0));
#endif // THAI_TEST

    // set text and shape the buffer
    hb_buffer_add_utf8(hbBuffer, textString, -1, 0, -1);
    hb_shape(hbFont, hbBuffer, NULL, 0);

    // extract shaped glyphs from the buffer
    int nGlyphs = hb_buffer_get_length(hbBuffer);
    hb_glyph_info_t *hbGlyphs = hb_buffer_get_glyph_infos(hbBuffer, NULL);
    hb_glyph_position_t *hbPositions = hb_buffer_get_glyph_positions(hbBuffer, NULL);

    // map shaped glyph indices and position for Qt's glyph run
    QVector<quint32> glyphIndexes(nGlyphs);
    QVector<QPointF> glyphPositions(nGlyphs);

    qreal x = 0.0, y = 0.0;
    for (int i = 0; i < nGlyphs; i++, hbGlyphs++, hbPositions++) {
        glyphIndexes[i] = hbGlyphs->codepoint;
        glyphPositions[i] = QPointF(x + hbPositions->x_offset, y - hbPositions->y_offset) / fontScale;
        x += hbPositions->x_advance;
        y -= hbPositions->y_advance;
    }

    // prepare Qt glyph run and raw font to draw the glyphs
    QRawFont rawFont = QRawFont(QString(fileName), fontSize);
    QGlyphRun glyphRun = QGlyphRun();

    glyphRun.setRawFont(rawFont);
    glyphRun.setGlyphIndexes(glyphIndexes);
    glyphRun.setPositions(glyphPositions);

    // voilà, draw the glyphs to the painter
    painter.drawGlyphRun(QPointF(marginSize, rawFont.ascent() + marginSize), glyphRun);

    // fit the window to the drawn glyphs
    resize(x/fontScale + marginSize*2, rawFont.ascent() + rawFont.descent() + marginSize*2);

    hb_buffer_destroy(hbBuffer);
    hb_font_destroy(hbFont);
    FT_Done_FreeType(ftLibrary);
}
