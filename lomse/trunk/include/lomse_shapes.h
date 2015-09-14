//---------------------------------------------------------------------------------------
// This file is part of the Lomse library.
// Copyright (c) 2010-2013 Cecilio Salmeron. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright notice, this
//      list of conditions and the following disclaimer.
//
//    * Redistributions in binary form must reproduce the above copyright notice, this
//      list of conditions and the following disclaimer in the documentation and/or
//      other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
// SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
// TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
// BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
// ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
// DAMAGE.
//
// For any comment, suggestion or feature request, please contact the manager of
// the project at cecilios@users.sourceforge.net
//---------------------------------------------------------------------------------------

#ifndef __LOMSE_SHAPES_H__        //to avoid nested includes
#define __LOMSE_SHAPES_H__

#include "lomse_shape_base.h"
#include "lomse_injectors.h"
#include "lomse_image.h"

namespace lomse
{

//forward declarations
class FontStorage;
class ImoButton;
class ImoImage;
class ImoStyle;

//---------------------------------------------------------------------------------------
// a shape drawn by using a single glyph from LenMus font
class GmoShapeGlyph : public GmoSimpleShape
{
protected:
    unsigned int m_glyph;
    USize m_shiftToDraw;
    FontStorage* m_pFontStorage;
    LibraryScope& m_libraryScope;
    double m_fontHeight;

public:
    virtual ~GmoShapeGlyph() {}

    virtual void on_draw(Drawer* pDrawer, RenderOptions& opt);

protected:
    GmoShapeGlyph(ImoObj* pCreatorImo, int type, ShapeId idx, unsigned int nGlyph,
                  UPoint pos, Color color, LibraryScope& libraryScope,
                  double fontHeight);

    void compute_size_origin(double fontHeight, UPoint pos);

};

//---------------------------------------------------------------------------------------
class GmoShapeClef : public GmoShapeGlyph
{
//protected:
//    friend class ClefEngraver;
public: //TO_FIX: constructor used in tests.
    GmoShapeClef(ImoObj* pCreatorImo, ShapeId idx, int nGlyph, UPoint pos, Color color,
                 LibraryScope& libraryScope, double fontSize)
        : GmoShapeGlyph(pCreatorImo, GmoObj::k_shape_clef, idx, nGlyph, pos, color,
                        libraryScope, fontSize )
    {
    }
};

//---------------------------------------------------------------------------------------
class GmoShapeImage : public GmoSimpleShape
{
protected:
    SpImage m_image;


    //TO_FIX: Classes derived from Control can not access the constructor!
    //friend class ImageEngrouter;
    //friend class Control;
public:
    GmoShapeImage(ImoObj* pCreatorImo, SpImage image, UPoint pos, USize size);
    void set_image(SpImage image);

public:
    void on_draw(Drawer* pDrawer, RenderOptions& opt);
};

//---------------------------------------------------------------------------------------
class GmoShapeFermata : public GmoShapeGlyph, public VoiceRelatedShape
{
protected:
    friend class FermataEngraver;
    GmoShapeFermata(ImoObj* pCreatorImo, ShapeId idx, int nGlyph, UPoint pos,
                    Color color, LibraryScope& libraryScope, double fontSize)
        : GmoShapeGlyph(pCreatorImo, GmoObj::k_shape_fermata, idx, nGlyph, pos, color,
                        libraryScope, fontSize )
        , VoiceRelatedShape()
    {
    }
};

//---------------------------------------------------------------------------------------
class GmoShapeSimpleLine : public GmoSimpleShape
{
protected:
    LUnits		m_uWidth;
	LUnits		m_uBoundsExtraWidth;
	ELineEdge	m_nEdge;

public:
    virtual ~GmoShapeSimpleLine() {}

    //implementation of virtual methods from base class
    void on_draw(Drawer* pDrawer, RenderOptions& opt);

protected:
    GmoShapeSimpleLine(ImoObj* pCreatorImo, int type, LUnits xStart, LUnits yStart,
                       LUnits xEnd, LUnits yEnd, LUnits uWidth, LUnits uBoundsExtraWidth,
                       Color color, ELineEdge nEdge = k_edge_normal);
    void set_new_values(LUnits xStart, LUnits yStart, LUnits xEnd, LUnits yEnd,
                        LUnits uWidth, LUnits uBoundsExtraWidth,
                        Color color, ELineEdge nEdge);

};

//---------------------------------------------------------------------------------------
class GmoShapeRectangle : public GmoSimpleShape
{
protected:
    LUnits m_radius;

public:
    GmoShapeRectangle(ImoObj* pCreatorImo
                      , int type = GmoObj::k_shape_rectangle
                      , ShapeId idx = 0
                      , const UPoint& pos = UPoint(0.0f, 0.0f)    //top-left corner
                      , const USize& size = USize(0.0f, 0.0f)     //rectangle size
                      , LUnits radius = 0.0f                      //for rounded corners
                      , ImoStyle* pStyle = NULL       //for line style & background color
                     );
    virtual ~GmoShapeRectangle() {}


    //implementation of virtual methods from base class
    virtual void on_draw(Drawer* pDrawer, RenderOptions& opt);

    //settings
    inline void set_radius(LUnits radius) { m_radius = radius; }

};

//---------------------------------------------------------------------------------------
class GmoShapeInvisible : public GmoSimpleShape
{
protected:
    friend class InvisibleEngraver;
    GmoShapeInvisible(ImoObj* pCreatorImo, ShapeId idx, UPoint uPos, USize uSize);

public:
    virtual void on_draw(Drawer* pDrawer, RenderOptions& opt);
};

//---------------------------------------------------------------------------------------
class GmoShapeStem : public GmoShapeSimpleLine, public VoiceRelatedShape
{
private:
	bool m_fStemDown;
    LUnits m_uExtraLength;

protected:
    friend class StemFlagEngraver;
    GmoShapeStem(ImoObj* pCreatorImo, LUnits xPos, LUnits yStart, LUnits uExtraLength,
                 LUnits yEnd, bool fStemDown, LUnits uWidth, Color color);

public:
    void change_length(LUnits length);
	inline bool is_stem_down() const { return m_fStemDown; }
    void set_stem_up(LUnits xRight, LUnits yNote);
    void set_stem_down(LUnits xLeft, LUnits yNote);
	void adjust(LUnits xLeft, LUnits yTop, LUnits height, bool fStemDown);
    inline LUnits get_extra_length() { return m_uExtraLength; }
    LUnits get_y_note();
    LUnits get_y_flag();
};

////---------------------------------------------------------------------------------------
//class GmoShapeFiguredBass : public lmCompositeShape
//{
//public:
//    GmoShapeFiguredBass(GmoBox* owner, int nShapeIdx, Color nColor)
//        : lmCompositeShape(pOwner, nShapeIdx, nColor, _T("Figured bass"), true)  //true= lmDRAGGABLE
//        { m_nType = GmoObj::k_shape_FiguredBass; }
//    ~GmoShapeFiguredBass() {}
//
//	//info about related shapes
//    inline void OnFBLineAttached(int nLine, GmoShapeLine* pShapeFBLine)
//                    { m_pFBLineShape[nLine] = pShapeFBLine; }
//
//    //overrides
//    void Shift(LUnits uxIncr, LUnits uyIncr);
//
//private:
//    GmoShapeLine*  m_pFBLineShape[2];     //The two lines of a FB line. This is the end FB of the line
//
//};
//
////---------------------------------------------------------------------------------------
//class GmoShapeWindow : public GmoShapeRectangle
//{
//public:
//    GmoShapeWindow(GmoBox* owner, int nShapeIdx,
//                  //position and size
//                  LUnits uxLeft, LUnits uyTop, LUnits uxRight, LUnits uyBottom,
//                  //border
//                  LUnits uBorderWidth, Color nBorderColor,
//                  //content
//                  Color nBgColor = *wxWHITE,
//                  //other
//                  wxString sName = _T("Window"),
//				  bool fDraggable = true, bool fSelectable = true,
//                  bool fVisible = true);
//    virtual ~GmoShapeWindow() {}
//
//    //renderization
//    void on_draw(Drawer* pDrawer, RenderOptions& opt);
//
//	//specific methods
//
//protected:
//
//    wxWindow*       m_pControl;      //the window to embbed
//};
//
////global functions defined in this module
//extern wxBitmap* GetBitmapForGlyph(double rScale, int nGlyph, double rPointSize,
//                                   Color colorF, Color colorB);
//

//---------------------------------------------------------------------------------------
class GmoShapeAccidentals : public GmoCompositeShape, public VoiceRelatedShape
{
protected:
    friend class AccidentalsEngraver;
    GmoShapeAccidentals(ImoObj* pCreatorImo, ShapeId idx, UPoint pos, Color color)
        : GmoCompositeShape(pCreatorImo, GmoObj::k_shape_accidentals, idx, color)
        , VoiceRelatedShape()
    {
    }
};

//---------------------------------------------------------------------------------------
class GmoShapeAccidental : public GmoShapeGlyph, public VoiceRelatedShape
{
protected:
    friend class AccidentalsEngraver;
    friend class KeyEngraver;
    GmoShapeAccidental(ImoObj* pCreatorImo, ShapeId idx, unsigned int iGlyph, UPoint pos,
                       Color color, LibraryScope& libraryScope, double fontSize)
        : GmoShapeGlyph(pCreatorImo, GmoObj::k_shape_accidental_sign, idx, iGlyph,
                        pos, color, libraryScope, fontSize)
        , VoiceRelatedShape()
    {
    }
};

//---------------------------------------------------------------------------------------
class GmoShapeTimeGlyph : public GmoShapeGlyph
{
protected:
    friend class TimeEngraver;
    GmoShapeTimeGlyph(ImoObj* pCreatorImo, ShapeId idx, unsigned int iGlyph, UPoint pos,
                  Color color, LibraryScope& libraryScope, double fontSize)
        : GmoShapeGlyph(pCreatorImo, GmoObj::k_shape_time_signature_glyph, idx, iGlyph,
                        pos, color, libraryScope, fontSize)
    {
    }
};

//---------------------------------------------------------------------------------------
class GmoShapeKeySignature : public GmoCompositeShape
{
protected:
    LibraryScope& m_libraryScope;

    friend class KeyEngraver;
    GmoShapeKeySignature(ImoObj* pCreatorImo, ShapeId idx, UPoint pos, Color color,
                         LibraryScope& libraryScope)
        : GmoCompositeShape(pCreatorImo, GmoObj::k_shape_key_signature, idx, color)
        , m_libraryScope(libraryScope)
    {
    }
};

//---------------------------------------------------------------------------------------
class GmoShapeTimeSignature : public GmoCompositeShape
{
protected:
    LibraryScope& m_libraryScope;

    friend class TimeEngraver;
    GmoShapeTimeSignature(ImoObj* pCreatorImo, ShapeId idx, UPoint pos,
                          Color color, LibraryScope& libraryScope)
        : GmoCompositeShape(pCreatorImo, GmoObj::k_shape_time_signature, idx, color)
        , m_libraryScope(libraryScope)
    {
    }
};

//---------------------------------------------------------------------------------------
class GmoShapeMetronomeMark : public GmoCompositeShape
{
protected:
    LibraryScope& m_libraryScope;

    friend class MetronomeMarkEngraver;
    GmoShapeMetronomeMark(ImoObj* pCreatorImo, ShapeId idx, UPoint pos, Color color,
                          LibraryScope& libraryScope)
        : GmoCompositeShape(pCreatorImo, GmoObj::k_shape_metronome_mark, idx, color)
        , m_libraryScope(libraryScope)
    {
    }
};

//---------------------------------------------------------------------------------------
class GmoShapeMetronomeGlyph : public GmoShapeGlyph
{
protected:
    friend class MetronomeMarkEngraver;
    GmoShapeMetronomeGlyph(ImoObj* pCreatorImo, ShapeId idx, unsigned int iGlyph, UPoint pos,
                           Color color, LibraryScope& libraryScope, double fontSize)
        : GmoShapeGlyph(pCreatorImo, GmoObj::k_shape_metronome_glyph, idx, iGlyph,
                        pos, color, libraryScope, fontSize)
    {
    }
};


}   //namespace lomse

#endif    // __LOMSE_SHAPES_H__
