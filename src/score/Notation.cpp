//--------------------------------------------------------------------------------------
//    LenMus Phonascus: The teacher of music
//    Copyright (c) 2002-2007 Cecilio Salmeron
//
//    This program is free software; you can redistribute it and/or modify it under the 
//    terms of the GNU General Public License as published by the Free Software Foundation;
//    either version 2 of the License, or (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful, but WITHOUT ANY 
//    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 
//    PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along with this 
//    program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, 
//    Fifth Floor, Boston, MA  02110-1301, USA.
//
//    For any comment, suggestion or feature request, please contact the manager of 
//    the project at cecilios@users.sourceforge.net
//
//-------------------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "Notation.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif


#include "Score.h"
#include "Notation.h"


lmNotation::lmNotation(lmVStaff* pVStaff, int nStaff, bool fVisible, bool fIsDraggable)
    : lmStaffObj(pVStaff, eSFOT_Notation, pVStaff, nStaff, fVisible, fIsDraggable)
{
}


//-----------------------------------------------------------------------------------------
// lmSpacer implementation
//-----------------------------------------------------------------------------------------

lmSpacer::lmSpacer(lmVStaff* pStaff, lmTenths nWidth, int nStaff)
    : lmNotation(pStaff, nStaff, true, (nWidth > 0))
{
    m_nSpacerWidth = nWidth;
}

void lmSpacer::DrawObject(bool fMeasuring, lmPaper* pPaper, wxColour colorC,
                                  bool fHighlight)
{
    if (fMeasuring) {
        // set total width
        m_uWidth = m_pVStaff->TenthsToLogical(m_nSpacerWidth, m_nStaffNum);

        // store glyph position. As it is relative to paper pos, it is always zero.
        //m_uGlyphPos.x = m_pVStaff->TenthsToLogical(m_uWidth, m_nStaffNum);
        m_uGlyphPos.x = 0;
        m_uGlyphPos.y = 0;

         // store selection rectangle (relative to m_uPaperPos).
        //  Coincides with glyph rectangle. Height is arbitrary: staff height.
        m_uSelRect.width = m_uWidth;
        m_uSelRect.height = 50;      // staff height: 5 lines
        m_uSelRect.x = m_uGlyphPos.x;
        m_uSelRect.y = m_uGlyphPos.y;
    }
    else {
        // Drawing phase. Nothing to do
    }

}

wxString lmSpacer::Dump()
{
    wxString sDump = wxString::Format(
        _T("%d\tSpacer %.2f\tTimePos=%.2f, fixed=%s\n"),
        m_nId, m_nSpacerWidth, m_rTimePos, (m_fFixedPos ? _T("yes") : _T("no")) );
    return sDump;
            
}

wxString lmSpacer::SourceLDP()
{
    wxString sSource = wxString::Format(_T("         (spacer %.0f)\n"), m_nSpacerWidth);
    return sSource;

}

wxString lmSpacer::SourceXML()
{
    //! @todo all
    wxString sSource = _T("TODO: lmSpacer XML Source code generation methods");
    return sSource;

}
