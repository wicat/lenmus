//--------------------------------------------------------------------------------------
//    LenMus Phonascus: The teacher of music
//    Copyright (c) 2002-2009 LenMus project
//
//    This program is free software; you can redistribute it and/or modify it under the
//    terms of the GNU General Public License as published by the Free Software Foundation,
//    either version 3 of the License, or (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful, but WITHOUT ANY
//    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
//    PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along with this
//    program. If not, see <http://www.gnu.org/licenses/>.
//
//    For any comment, suggestion or feature request, please contact the manager of
//    the project at cecilios@users.sourceforge.net
//
//-------------------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "ToolSymbols.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#else
#include <wx/xrc/xmlres.h>
#include <wx/bmpcbox.h>
#include <wx/statline.h>
#include <wx/clrpicker.h>
#endif

#include "ToolsBox.h"
#include "ToolSymbols.h"
#include "ToolGroup.h"
#include "../ArtProvider.h"         //to use ArtProvider for managing icons
#include "../TheApp.h"              //to use GetMainFrame()
#include "../MainFrame.h"           //to get active lmScoreCanvas
#include "../ScoreCanvas.h"         //to send commands
#include "../../widgets/Button.h"
#include "../../score/defs.h"
#include "../../score/FiguredBass.h"
#include "../../graphic/GraphicManager.h"   //to use GenerateBitmapForKeyCtrol()



#define lmSPACING 5

#define lm_NUM_SYMBOL_BUTTONS 4

//event IDs
enum {
    lmID_BT_Symbols = 2600,
};


IMPLEMENT_DYNAMIC_CLASS(lmToolPageSymbols, lmToolPage)


lmToolPageSymbols::lmToolPageSymbols()
{
}

lmToolPageSymbols::lmToolPageSymbols(wxWindow* parent)
{
    Create(parent);
}

void lmToolPageSymbols::Create(wxWindow* parent)
{
    //base class
    lmToolPage::Create(parent);

    //initialize data
    m_sPageToolTip = _("Edit tools for text, graphics and symbols");
    m_sPageBitmapName = _T("tool_symbols");

    //create groups
    CreateGroups();
}

lmToolPageSymbols::~lmToolPageSymbols()
{
    delete m_pGrpSymbols;
}

lmToolGroup* lmToolPageSymbols::GetToolGroup(lmEToolGroupID nGroupID)
{
    switch(nGroupID)
    {
        case lmGRP_Symbols:    return m_pGrpSymbols;
        default:
            wxASSERT(false);
    }
    return (lmToolGroup*)NULL;      //compiler happy
}

void lmToolPageSymbols::CreateGroups()
{
    //Create the groups for this page

    wxBoxSizer *pMainSizer = GetMainSizer();

    m_pGrpSymbols = new lmGrpSymbols(this, pMainSizer);

	CreateLayout();
}



//--------------------------------------------------------------------------------
// lmGrpSymbols implementation
//--------------------------------------------------------------------------------

lmGrpSymbols::lmGrpSymbols(lmToolPage* pParent, wxBoxSizer* pMainSizer)
        : lmToolButtonsGroup(pParent, lm_NUM_SYMBOL_BUTTONS, lmTBG_ONE_SELECTED, pMainSizer,
                             lmID_BT_Symbols, pParent->GetColors())
{
    CreateControls(pMainSizer);
}

void lmGrpSymbols::CreateControls(wxBoxSizer* pMainSizer)
{
    //create the buttons for the group

    lmToolButtonData cButtons[] =
    {
        { lmTOOL_FIGURED_BASS,  _("Figured bass"),          _T("symbols_figured_bass") },
        { lmTOOL_TEXT,          _("Text"),                  _T("symbols_text") },
        { lmTOOL_LINES,         _("Lines and arrows"),      _T("symbols_line") },
        { lmTOOL_TEXTBOX,       _("Text boxes"),            _T("symbols_textbox") },
    };

    int nNumButtons = sizeof(cButtons) / sizeof(lmToolButtonData);

    wxBoxSizer* pCtrolsSizer = CreateGroup(pMainSizer, _T("")); //_("Text, symbols, graphics"));

    SetFont(wxFont(8, wxSWISS, wxNORMAL, wxNORMAL, false, wxT("Tahoma")));

    wxBoxSizer* pButtonsSizer;
    wxSize btSize(24, 24);
	for (int iB=0; iB < nNumButtons; iB++)
	{
		//if (iB % 9 == 0) {
			pButtonsSizer = new wxBoxSizer(wxHORIZONTAL);
			pCtrolsSizer->Add(pButtonsSizer);
		//}

		wxString sBtName = cButtons[iB].sBitmapName;
		m_pButton[iB] = new lmCheckButton(this, lmID_BT_Symbols+iB, wxBitmap(24, 24));
        m_pButton[iB]->SetBitmapUp(sBtName, _T(""), btSize);
        m_pButton[iB]->SetBitmapDown(sBtName, _T("button_selected_flat"), btSize);
        m_pButton[iB]->SetBitmapOver(sBtName, _T("button_over_flat"), btSize);
		m_pButton[iB]->SetToolTip(cButtons[iB].sToolTip);
		pButtonsSizer->Add(m_pButton[iB], wxSizerFlags(0).Border(wxALL, 0) );
		pButtonsSizer->Add( new wxStaticText(this, wxID_ANY, cButtons[iB].sToolTip),
                            wxSizerFlags(0).Border(wxLEFT|wxTOP|wxBOTTOM, 5) );
	}

	this->Layout();

	SelectButton(0);	//select figured bass button
}
