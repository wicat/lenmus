//--------------------------------------------------------------------------------------
//    LenMus Phonascus: The teacher of music
//    Copyright (c) 2002-2010 LenMus project
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

#ifndef __LM_TOOLNOTES_H__
#define __LM_TOOLNOTES_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "ToolNotes.cpp"
#endif

#include "ToolGroup.h"
#include "ToolPage.h"
#include "../../score/defs.h"

class wxBitmapComboBox;
class lmCheckButton;
class lmBitmapButton;


//--------------------------------------------------------------------------------
// Group for octave number
//--------------------------------------------------------------------------------
class lmGrpOctave : public lmToolButtonsGroup
{
public:
    lmGrpOctave(lmToolPage* pParent, wxBoxSizer* pMainSizer);
    ~lmGrpOctave() {}

    //implement virtual methods
    void CreateGroupControls(wxBoxSizer* pMainSizer);
    inline lmEToolGroupID GetToolGroupID() { return lmGRP_Octave; }

	//access to options
	inline int GetOctave() { return m_nSelButton; }
	inline void SetOctave(int nOctave) { SelectButton(nOctave); }
    void SetOctave(bool fUp);

};


//--------------------------------------------------------------------------------
// Group for voice number: base class
//--------------------------------------------------------------------------------
class lmGrpVoice : public lmToolButtonsGroup
{
public:
    ~lmGrpVoice() {}

    //implement virtual methods
    //void CreateGroupControls(wxBoxSizer* pMainSizer) = 0;
    inline lmEToolGroupID GetToolGroupID() { return lmGRP_Voice; }

	//access to options
	virtual int GetVoice() { return m_nSelButton; }
	virtual void SetVoice(int nVoice) { SelectButton(nVoice); }
    void SetVoice(bool fUp);

protected:
    lmGrpVoice(lmToolPage* pParent, wxBoxSizer* pMainSizer, int nNumButtons);

};

//--------------------------------------------------------------------------------
// Group for voice number: standard group
//--------------------------------------------------------------------------------
class lmGrpVoiceStd : public lmGrpVoice
{
public:
    lmGrpVoiceStd(lmToolPage* pParent, wxBoxSizer* pMainSizer);
    ~lmGrpVoiceStd() {}

    //implement virtual methods
    void CreateGroupControls(wxBoxSizer* pMainSizer);
};

//--------------------------------------------------------------------------------
// Group for voice number: for harmony exercises
//--------------------------------------------------------------------------------
class lmGrpVoiceHarmony : public lmGrpVoice
{
public:
    lmGrpVoiceHarmony(lmToolPage* pParent, wxBoxSizer* pMainSizer);
    ~lmGrpVoiceHarmony() {}

    //implement virtual methods
    void CreateGroupControls(wxBoxSizer* pMainSizer);

    //overrides, to avoide voice 0 (AutoVoice)
    int GetVoice() { return m_nSelButton + 1; }
	void SetVoice(int nVoice) { SelectButton(nVoice - 1); }

};



//--------------------------------------------------------------------------------
// Group for Note duration
//--------------------------------------------------------------------------------
class lmGrpNoteDuration : public lmToolButtonsGroup
{
public:
    lmGrpNoteDuration(lmToolPage* pParent, wxBoxSizer* pMainSizer);
    ~lmGrpNoteDuration() {}

    //implement virtual methods
    void CreateGroupControls(wxBoxSizer* pMainSizer);
    inline lmEToolGroupID GetToolGroupID() { return lmGRP_NoteDuration; }

	//access to options
	lmENoteType GetNoteDuration();

    //modify buttons
    void SetButtonsBitmaps(bool fNotes);

};



//--------------------------------------------------------------------------------
// Group to select notes or rests
//--------------------------------------------------------------------------------
class lmGrpNoteRest : public lmToolButtonsGroup
{
public:
    lmGrpNoteRest(lmToolPage* pParent, wxBoxSizer* pMainSizer);
    ~lmGrpNoteRest() {}

    //implement virtual methods
    void CreateGroupControls(wxBoxSizer* pMainSizer);
    inline lmEToolGroupID GetToolGroupID() { return lmGRP_NoteRest; }

	//access to options
	bool IsNoteSelected();
    inline bool IsRestSelected() { return !IsNoteSelected(); }

};


//--------------------------------------------------------------------------------
// Group for Note accidentals
//--------------------------------------------------------------------------------
class lmGrpNoteAcc : public lmToolButtonsGroup
{
public:
    lmGrpNoteAcc(lmToolPage* pParent, wxBoxSizer* pMainSizer);
    ~lmGrpNoteAcc() {}

    //implement virtual methods
    void CreateGroupControls(wxBoxSizer* pMainSizer);
    inline lmEToolGroupID GetToolGroupID() { return lmGRP_NoteAcc; }

	//access to options
	lmEAccidentals GetNoteAcc();

};


//--------------------------------------------------------------------------------
// Group for note dots
//--------------------------------------------------------------------------------
class lmGrpNoteDots : public lmToolButtonsGroup
{
public:
    lmGrpNoteDots(lmToolPage* pParent, wxBoxSizer* pMainSizer);
    ~lmGrpNoteDots() {}

    //implement virtual methods
    void CreateGroupControls(wxBoxSizer* pMainSizer);
    inline lmEToolGroupID GetToolGroupID() { return lmGRP_NoteDots; }

	//access to options
	int GetNoteDots();

};


//--------------------------------------------------------------------------------
// Group for tuplets, ties, ...
//--------------------------------------------------------------------------------
class lmGrpNoteModifiers : public lmToolGroup
{
public:
    lmGrpNoteModifiers(lmToolPage* pParent, wxBoxSizer* pMainSizer);
    ~lmGrpNoteModifiers() {}

    //implement virtual methods
    void CreateGroupControls(wxBoxSizer* pMainSizer);
    inline lmEToolGroupID GetToolGroupID() { return lmGRP_NoteModifiers; }
    inline lmEToolID GetCurrentToolID() { return m_nSelectedToolID; }

    //event handlers
    void OnTieButton(wxCommandEvent& event);
    void OnTupletButton(wxCommandEvent& event);
    void OnToggleStemButton(wxCommandEvent& event);

	//access to options
    void SetToolTie(bool fChecked);
    void SetToolTuplet(bool fChecked);
    void SetToolToggleStem(bool fChecked);
    void EnableTool(lmEToolID nToolID, bool fEnabled);


protected:
    lmCheckButton*      m_pBtnTie;
    lmCheckButton*      m_pBtnTuplet;
    lmCheckButton*      m_pBtnToggleStem;
    lmEToolID           m_nSelectedToolID;      //clicked tool

    DECLARE_EVENT_TABLE()
};


//--------------------------------------------------------------------------------
// Group for beams
//--------------------------------------------------------------------------------
class lmGrpBeams : public lmToolGroup
{
public:
    lmGrpBeams(lmToolPage* pParent, wxBoxSizer* pMainSizer);
    ~lmGrpBeams() {}

    //implement virtual methods
    void CreateGroupControls(wxBoxSizer* pMainSizer);
    inline lmEToolGroupID GetToolGroupID() { return lmGRP_Beams; }
    inline lmEToolID GetCurrentToolID() { return m_nSelectedToolID; }

    //event handlers
    void OnButton(wxCommandEvent& event);

    void EnableTool(lmEToolID nToolID, bool fEnabled);


protected:
    //void PostToolBoxEvent(lmEToolID nToolID, bool fSelected);

    lmBitmapButton*     m_pBtnBeamCut;
    lmBitmapButton*     m_pBtnBeamJoin;
    lmBitmapButton*     m_pBtnBeamFlatten;
    lmBitmapButton*     m_pBtnBeamSubgroup;
    lmEToolID           m_nSelectedToolID;      //clicked tool

    DECLARE_EVENT_TABLE()
};



//--------------------------------------------------------------------------------
// lmToolPageNotes: Abstract class to build specific Note tools pages
//--------------------------------------------------------------------------------

class lmToolPageNotes : public lmToolPage
{
	DECLARE_ABSTRACT_CLASS(lmToolPageNotes)

public:
    virtual ~lmToolPageNotes();

    //creation
    virtual void CreateGroups() = 0;
    virtual void Create(wxWindow* parent);

	//access to options
    wxString GetToolShortDescription();

    //interface with Octave group
	inline int GetOctave() { return m_pGrpOctave->GetOctave(); }
    inline void SetOctave(bool fUp) { m_pGrpOctave->SetOctave(fUp); }
    inline void SetOctave(int nOctave) { m_pGrpOctave->SetOctave(nOctave); }

    //interface with voice group
	inline int GetVoice() { return m_pGrpVoice->GetVoice(); }
    inline void SetVoice(bool fUp) { m_pGrpVoice->SetVoice(fUp); }
    inline void SetVoice(int nVoice) { m_pGrpVoice->SetVoice(nVoice); }

    //interface with Note/Rest group
	inline bool IsNoteSelected() { return m_pGrpNoteRest->IsNoteSelected(); }
    inline bool IsRestSelected() { return m_pGrpNoteRest->IsRestSelected(); }

    //interface with NoteDuration group
    inline void EnableGrpNoteDuration(bool fEnabled) { m_pGrpNoteDuration->EnableGroup(fEnabled); }
    inline void SetNoteDuration(lmENoteType nNoteType) { m_pGrpNoteDuration->SelectButton((int)nNoteType - 1); }
    inline lmENoteType GetNoteDuration() { return m_pGrpNoteDuration->GetNoteDuration(); }
    inline int GetNoteDurationButton() { return m_pGrpNoteDuration->GetSelectedButton(); }
    inline void SetNoteDurationButton(int iB) { m_pGrpNoteDuration->SelectButton(iB); }

    //interface with NoteAccidentals group
    inline void EnableGrpNoteAcc(bool fEnabled) { m_pGrpNoteAcc->EnableGroup(fEnabled); }
    inline void SetNoteAccidentals(lmEAccidentals nAcc) { m_pGrpNoteAcc->SelectButton((int)nAcc - 1); }
    inline lmEAccidentals GetNoteAccidentals() { return m_pGrpNoteAcc->GetNoteAcc(); }
    inline void SelectNextAccidental() { m_pGrpNoteAcc->SelectNextButton(); }
    inline void SelectPrevAccidental() { m_pGrpNoteAcc->SelectPrevButton(); }
    inline int GetNoteAccButton() { return m_pGrpNoteAcc->GetSelectedButton(); }
    inline void SetNoteAccButton(int iB) { m_pGrpNoteAcc->SelectButton(iB); }

    //interface with NoteDots group
    inline void EnableGrpNoteDots(bool fEnabled) { m_pGrpNoteDots->EnableGroup(fEnabled); }
    inline void SetNoteDots(int nDots) { m_pGrpNoteDots->SelectButton(nDots - 1); }
    inline int GetNoteDots() { return m_pGrpNoteDots->GetNoteDots(); }
    inline void SelectNextDot() { m_pGrpNoteDots->SelectNextButton(); }
    inline void SelectPrevDot() { m_pGrpNoteDots->SelectPrevButton(); }
    inline int GetNoteDotsButton() { return m_pGrpNoteDots->GetSelectedButton(); }
    inline void SetNoteDotsButton(int iB) { m_pGrpNoteDots->SelectButton(iB); }

    //interface with Modifiers group
    inline void EnabelGrpModifiers(bool fEnabled) { m_pGrpModifiers->EnableGroup(fEnabled); }
    inline void SetToolTie(bool fChecked) { m_pGrpModifiers->SetToolTie(fChecked); }
    inline void SetToolTuplet(bool fChecked) { m_pGrpModifiers->SetToolTuplet(fChecked); }
    inline void SetToolToggleStem(bool fChecked) { m_pGrpModifiers->SetToolToggleStem(fChecked); }

    //interface with Beam tools group
    //inline void EnabelGrpModifiers(bool fEnabled) { m_pGrpModifiers->EnableGroup(fEnabled); }
    //inline void SetToolTie(bool fChecked) { m_pGrpModifiers->SetToolTie(fChecked); }
    //inline void SetToolTuplet(bool fChecked) { m_pGrpModifiers->SetToolTuplet(fChecked); }


    //interface with NoteheadType group
	lmENoteHeads GetNoteheadType();
    //inline lmENoteHeads GetNoteheadType() { return m_pGrpNoteDuration->GetNoteDuration(); }
    //inline void SetNoteDurationButton(int iB) { m_pGrpNoteDuration->SelectButton(iB); }


protected:
    lmToolPageNotes(wxWindow* parent);
    lmToolPageNotes();

    //groups
    lmGrpNoteRest*      m_pGrpNoteRest;
    lmGrpNoteDuration*  m_pGrpNoteDuration;
    lmGrpNoteAcc*       m_pGrpNoteAcc;
    lmGrpNoteDots*      m_pGrpNoteDots;
    lmGrpNoteModifiers*     m_pGrpModifiers;
    lmGrpBeams*         m_pGrpBeams;
	lmGrpOctave*		m_pGrpOctave;
	lmGrpVoice*			m_pGrpVoice;
	//lmGrpMouseMode*     m_pGrpEntryMode;

	//options
	wxBitmapComboBox*	m_pCboNotehead;
	wxBitmapComboBox*	m_pCboAccidentals;
};



//--------------------------------------------------------------------------------
// lmToolPageNotesStd: Standard page for Notes tools
//--------------------------------------------------------------------------------

class lmToolPageNotesStd : public lmToolPageNotes
{
	DECLARE_DYNAMIC_CLASS(lmToolPageNotesStd)

public:
    lmToolPageNotesStd(wxWindow* parent);
    lmToolPageNotesStd();
    ~lmToolPageNotesStd();

    //implementation of pure virtual base class methods
    void Create(wxWindow* parent);
    void CreateGroups();


protected:

};



//--------------------------------------------------------------------------------
// lmToolPageNotesHarmony: Notes tools page for harmony exercises
//--------------------------------------------------------------------------------

class lmToolPageNotesHarmony : public lmToolPageNotes
{
	DECLARE_DYNAMIC_CLASS(lmToolPageNotesHarmony)

public:
    lmToolPageNotesHarmony(wxWindow* parent);
    lmToolPageNotesHarmony();                   //default, for dynamic creation
    ~lmToolPageNotesHarmony();

    //implementation of pure virtual base class methods
    void CreateGroups();
    void Create(wxWindow* parent);             //for dynamic creation

    //overrides
    wxMenu* GetContextualMenuForToolPage();
    void OnPopUpMenuEvent(wxCommandEvent& event);

protected:
    wxMenu*         m_pMenu;        //contextual menu

};


#endif    // __LM_TOOLNOTES_H__
