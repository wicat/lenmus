//---------------------------------------------------------------------------------------
// This file is part of the Lomse library.
// Copyright (c) 2010-2016 Cecilio Salmeron. All rights reserved.
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

#ifndef _LOMSE_SCORE_PLAYER_CTRL_H__
#define _LOMSE_SCORE_PLAYER_CTRL_H__

#include "lomse_control.h"
#include "lomse_player_gui.h"


namespace lomse
{

//forward declarations
class ImoScorePlayer;
class Metronome;


//---------------------------------------------------------------------------------------
// A Control containing a static text element and/or and image which links to an URL
class ScorePlayerCtrl : public Control, public PlayerGui
{
protected:
    ImoScorePlayer* m_pOwnerImo;
    GmoBoxControl* m_pMainBox;
    UPoint  m_pos;
    LUnits  m_width;
    LUnits  m_height;

    Color   m_normalColor;
    Color   m_hoverColor;
    Color   m_currentColor;

    int     m_metronome;
    int     m_playButtonState;
    bool    m_fFullView;

public:
    ScorePlayerCtrl(LibraryScope& libScope, ImoScorePlayer* pOwner, Document* pDoc);
    virtual ~ScorePlayerCtrl() {}

    //Control mandatory overrides
    USize measure();
    GmoBoxControl* layout(LibraryScope& libraryScope, UPoint pos);
    void on_draw(Drawer* pDrawer, RenderOptions& opt);
    void handle_event(SpEventInfo pEvent);
    LUnits width() { return m_width; }
    LUnits height() { return m_height; }
    LUnits top() { return m_pos.y; }
    LUnits bottom() { return m_pos.y + m_height; }
    LUnits left() { return m_pos.x; }
    LUnits right() { return m_pos.x + m_width; }

    //PlayerGui mandatory overrides
    void on_end_of_playback();
    int get_play_mode();
    int get_metronome_mm();
    Metronome* get_metronome();
    bool countoff_status();
    bool metronome_status();

    //specific methods
    void set_text(const string& UNUSED(text)) {}
    void set_tooltip(const string& text);
    inline void set_metronome_mm(int value) { m_metronome = value; }
    void set_play_button_state(int value);

    enum { k_stop=0, k_pause, k_play};  //play button state

protected:
    ImoStyle* create_default_style();

};


} //namespace lomse

#endif    //_LOMSE_SCORE_PLAYER_CTRL_H__
