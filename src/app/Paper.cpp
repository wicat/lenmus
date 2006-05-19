//--------------------------------------------------------------------------------------
//    LenMus Phonascus: The teacher of music
//    Copyright (c) 2002-2006 Cecilio Salmeron
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
/*! @file Paper.cpp
    @brief Implementation file for class lmPaper
    @ingroup app_gui
*/
/*! @class lmPaper
    @ingroup app_gui
    @brief The generic canvas on which the score is rendered.
    
    A 'lmPaper' is the generic canvas on which the score is rendered. It might be a display
    device,    a printer, etc. Basically it is a DC on wich to write and draw and some methods
    to deal with unit conversion.
    
    The lmPaper responsibilities are:
    - it manages the offscreen bitmaps that receives all drawing operations
    - it is a container for the DC object on which to write.
    - it is responsible for all scale and unit conversion methods
    - it is responsible for informing the staff objects about page margins, spacings, 
        layout, etc.

    For rendering a score the steps to follow are:
    1. The view must call Prepare(). Prepare is responsible for calling
        StartDoc(), NewPage() and then score->Draw()
    2. The draw process starts with a new page prepared and must call NewPage()
        for advancing the paper.
    3. When finishing, must not invoke NewPage().
    4. Then Prepare must call EndDoc() 

    ==> StartDoc() and EndDoc() are not public

*/
#ifdef __GNUG__
// #pragma implementation
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "scoreview.h"
#include "../score/Score.h"
#include "Page.h"
#include "FontManager.h"
#include "Paper.h"
#include "../graphic/GraphicManager.h"


// global data structures for printing. Defined in TheApp.cpp
#include "wx/cmndata.h"
extern wxPrintData* g_pPrintData;
extern wxPageSetupData* g_pPaperSetupData;

// Definition of the BitmapList class
#include <wx/listimpl.cpp>
WX_DEFINE_LIST(BitmapList);


lmPaper::lmPaper()
{
    m_xCursor = 0;
    m_yCursor = 0;
    m_pDC = (wxDC *) NULL;

    // bitmaps
    m_xPageSize = 0;
    m_yPageSize = 0;
    m_rScale = 0;
    m_numPages = 0;
    m_fRedraw = true;
    m_pScore = (lmScore*)NULL;
    m_nLastScoreID = -1;

}

lmPaper::~lmPaper()
{
    DeleteBitmaps();
    //m_cBitmaps.DeleteContents(true);    // so that Clear() willdelete all bitmaps
    //m_cBitmaps.Clear();                    // now, delete all elements
}

// returns paper size in logical units
wxSize& lmPaper::GetPaperSize()
{
    return m_Page.PageSize();
    //wxSize sz = g_pPaperSetupData->GetPaperSize();
    //return wxSize(10*sz.GetWidth(), 10*sz.GetHeight());
}


void lmPaper::Prepare(lmScore* pScore, lmLUnits paperWidth, lmLUnits paperHeight, double rScale)
{
    // If the score has changed or the scale has changed or paper size has changed 
    // we need to recreate the bitmaps
    bool fDrawScore = m_fRedraw;
    m_fRedraw = false;
    if (!m_pScore || m_nLastScoreID != pScore->GetID() || m_rScale != rScale || fDrawScore) {

        fDrawScore = true;

        //store new values
        m_pScore = pScore;
        m_nLastScoreID = m_pScore->GetID();
        m_rScale = rScale;
        m_xPageSize = paperWidth;
        m_yPageSize = paperHeight;

        // if size has changed delete the allocated bitmaps
        if (paperWidth != m_xBitmapSize || paperHeight != m_yBitmapSize) {
            DeleteBitmaps();
        }
    }

    //repaint score if necesary
    if (fDrawScore) {

        // Allocate a DC in memory for the offscreen bitmap
        wxMemoryDC memDC;
        SetDC(&memDC);

        m_numPages = 0;        // first page

        // Prepare the first page
        NewPage();

        // Ask the score to draw itself onto this lmPaper
        //m_pScore->Draw(this);
        // ask the graphics manager to render the score on this paper
        lmGraphicManager oGraphicMngr(m_pScore, this);
        oGraphicMngr.Layout();
        oGraphicMngr.Render(this);


        // @attention method lmScore.Draw() will call NewPage() when need a new page. This will
        // select a new bitmap into de memDC.

        ////DEBUG: draw red lines on last page to show cursor position
        //lmLUnits yCur = GetCursorY();
        //lmLUnits xCur = GetCursorX();
        //m_pDC->SetPen(*wxRED_PEN);
        //m_pDC->DrawLine(0, yCur, GetPaperSize().GetWidth(), yCur);
        //m_pDC->DrawLine(xCur, 0, xCur, GetPaperSize().GetHeight());
        ////End DEBUG --------------------------------------------

        // deselect last page bitmap
        if (m_pDC->IsKindOf(CLASSINFO(wxMemoryDC))) {
            ((wxMemoryDC*)m_pDC)->SelectObject(wxNullBitmap);
        }
        SetDC( (wxDC*)NULL );
    }

}

void lmPaper::RestartPageCursors()
{
    m_xCursor = GetPageLeftMargin();
    m_yCursor = GetPageTopMargin();

}

void lmPaper::NewLine(lmLUnits nSpace)
{
    m_yCursor += nSpace;
    m_xCursor = GetPageLeftMargin();
    
}



void lmPaper::NewPage()
{
    wxASSERT(m_pDC);
    RestartPageCursors();

    if (m_pDC->IsKindOf(CLASSINFO(wxMemoryDC))) {
        // get the bitmap for first page
        wxBitmap* pBitmap = GetPageBitmap(m_numPages);
        wxASSERT(pBitmap);
        wxASSERT(pBitmap->Ok());
        m_numPages++;

        ((wxMemoryDC *)m_pDC)->SelectObject(*pBitmap);
    }

    m_pDC->Clear();
    m_pDC->SetMapMode(lmDC_MODE);
    m_pDC->SetUserScale( m_rScale, m_rScale );

    ////DEBUG: draw green lines to show initial cursor position
    //if (m_numPages==1) {
    //    lmLUnits yCur = GetCursorY();
    //    lmLUnits xCur = GetCursorX();
    //    m_pDC->SetPen(*wxGREEN_PEN);
    //    m_pDC->DrawLine(0, yCur, GetPaperSize().GetWidth(), yCur);
    //    m_pDC->DrawLine(xCur, 0, xCur, GetPaperSize().GetHeight());

    //    m_pDC->DrawLine(0, GetPaperSize().GetHeight()-100, 
    //                    GetPaperSize().GetWidth(), 
    //                    GetPaperSize().GetHeight()-100);
    //}
    ////End DEBUG --------------------------------------------

}

lmLUnits lmPaper::GetRightMarginXPos()
{ 
    return GetPaperSize().GetWidth() - GetPageRightMargin();
}

lmLUnits lmPaper::GetLeftMarginXPos()
{
    return GetPageLeftMargin();
}

// Get the bitmap for page nPage. If no bitmap is allocated, do it.
// nPage = 0 .. n-1
wxBitmap* lmPaper::GetPageBitmap(wxInt32 nPage)
{
    wxASSERT(nPage >= 0);
    wxASSERT(nPage <= m_numPages);    // m_numPages could be not yet incremented

    wxBitmap* pBitmap;
    wxInt32 nNumBitmaps = m_cBitmaps.GetCount();
    if (nNumBitmaps > 0 && nNumBitmaps > nPage ) {
        // bitmap already exits. Get it.
        wxBitmapListNode* pNode = m_cBitmaps.Item(nPage);
        wxASSERT(pNode);
        pBitmap = pNode->GetData();

    } else {
        //No bitmap allocated. Create one
        pBitmap = new wxBitmap(m_xPageSize, m_yPageSize);
        wxLogMessage(_T("[lmPaper::GetPageBitmap] Allocated bitmap (%d, %d) pixels, %d bits/pixel. Size= %.02f MB"),
            m_xPageSize, m_yPageSize, pBitmap->GetDepth(), (double)((m_xPageSize * m_yPageSize * pBitmap->GetDepth())/8000000.) );
        if (!pBitmap || !pBitmap->Ok()) {
            if (pBitmap) {
                delete pBitmap;
                pBitmap = (wxBitmap *) NULL;
            }
            wxLogMessage(_T("lmPaper::GetPageBitmap: Bitmap size (%d, %d) pixels."), m_xPageSize, m_yPageSize);
            wxMessageBox(_("Sorry, not enough memory to create a pBitmap to display the page."),
                _T("lmPaper.GetPageBitmap"), wxOK);
            ::wxExit();
        }
        // add the new bitmap to the list and store its size
        m_cBitmaps.Append(pBitmap);
        m_xBitmapSize = m_xPageSize;
        m_yBitmapSize = m_yPageSize;

    }

    return pBitmap;
}

wxFont* lmPaper::GetFont(int nPointSize, wxString sFontName, 
                       int nFamily, int nStyle, int nWeight, bool fUnderline)
{
    return m_fontManager.GetFont(nPointSize, sFontName, nFamily, nStyle, nWeight, fUnderline);
}


// nPage = 0 ... n-1
wxBitmap* lmPaper::GetOffscreenBitmap(wxInt32 nPage)
{
    wxASSERT(nPage >=0 && nPage < m_numPages);
    wxASSERT(m_cBitmaps.GetCount());
    wxASSERT(m_cBitmaps.GetCount() > (size_t)nPage );

    wxBitmapListNode* pNode = m_cBitmaps.Item(nPage);
    wxASSERT(pNode);
    return pNode->GetData();
    
}

void lmPaper::DeleteBitmaps()
{
    wxBitmapListNode* pNode = m_cBitmaps.GetFirst();
    while (pNode) {
        wxBitmap* pBitmap = pNode->GetData();
        delete pBitmap;
        delete pNode;
        pNode = m_cBitmaps.GetFirst();
    }

}

