//--------------------------------------------------------------------------------------
//    LenMus Phonascus: The teacher of music
//    Copyright (c) 2002-2009 Cecilio Salmeron
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

#ifndef __LM_HARMONYCONSTRAINS_H__        //to avoid nested includes
#define __LM_HARMONYCONSTRAINS_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "HarmonyConstrains.cpp"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "Generators.h"
#include "Constrains.h"
#include "CadencesConstrains.h"

class lmHarmonyConstrains : public lmExerciseOptions
{
public:
    lmHarmonyConstrains(wxString sSection);
    ~lmHarmonyConstrains() {}

    lmECadenceType GetRandomCadence();

    bool IsCadenceValid(lmECadenceType nType) { return m_fValidCadences[nType]; }
    void SetCadenceValid(lmECadenceType nType, bool fValid) { m_fValidCadences[nType] = fValid; }
    bool* GetValidCadences() { return m_fValidCadences; }

    bool IsValidButton(lmECadenceButtons nB) { return m_fValidButtons[nB]; }
    void SetValidButton(lmECadenceButtons nB, bool fValue) { m_fValidButtons[nB] = fValue; }
    bool* GetValidButtons() { return m_fValidButtons; }

    int GetKeyDisplayMode() { return m_nKeyDisplayMode; }
    void SetKeyDisplayMode(int nKeyDisplayMode) { m_nKeyDisplayMode = nKeyDisplayMode; }

    void SetSection(wxString sSection) {
                m_sSection = sSection;
                LoadSettings();
            }

    void SaveSettings();
    
    lmKeyConstrains* GetKeyConstrains() { return &m_oValidKeys; }


private:
    void LoadSettings();

    bool                m_fValidButtons[lm_eCadMaxButton];
    bool                m_fValidCadences[lm_eCadMaxCadence];
    lmKeyConstrains     m_oValidKeys;           //allowed key signatures

    //params only for ear training exercises
    int                 m_nKeyDisplayMode;      // 0-play A4 note
                                                // 1-play tonic chord

};

#endif  // __LM_HARMONYCONSTRAINS_H__