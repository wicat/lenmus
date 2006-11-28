// RCS-ID: $Id: EarIntvalConstrains.h,v 1.5 2006/02/23 19:19:15 cecilios Exp $
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
/*! @file EarIntvalConstrains.h
    @brief Header file for lmEarIntervalsConstrains class
    @ingroup generators
*/
#ifdef __GNUG__
// #pragma interface
#endif

#ifndef __EARINTERVALSCONSTRAINS_H__        //to avoid nested includes
#define __EARINTERVALSCONSTRAINS_H__

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "../score/score.h"
#include "Constrains.h"

/*! @class lmEarIntervalsConstrains
    @brief Options for lmEarIntervalsCtrol control
*/
class lmEarIntervalsConstrains
{
public:
    lmEarIntervalsConstrains(wxString sSection);
    ~lmEarIntervalsConstrains() {}

    bool IsIntervalAllowed(int nInterval) { return m_fIntervalAllowed[nInterval]; }
    void SetIntervalAllowed(int nInterval, bool fValue) {
            m_fIntervalAllowed[nInterval] = fValue;
        }
    bool IsTypeAllowed(int nType) { return m_fTypeAllowed[nType]; }
    void SetTypeAllowed(int nType, bool fValue) {
            m_fTypeAllowed[nType] = fValue;
        }
    bool OnlyNatural() { return m_fOnlyNatural; }
    void SetOnlyNatural(bool fValue) { m_fOnlyNatural = fValue; }

    bool FirstNoteEqual() { return m_fFirstEqual; }
    void SetFirstNoteEqual(bool fValue) { m_fFirstEqual = fValue; }
    

    lmKeyConstrains* GetKeyConstrains() { return &m_oValidKeys; }

    bool* AllowedIntervals() { return m_fIntervalAllowed; }
    void SetMinNote(lmPitch nPitch) { m_nMinPitch = nPitch; }
    lmPitch MinNote() { return m_nMinPitch; }
    void SetMaxNote(lmPitch nPitch) { m_nMaxPitch = nPitch; }
    lmPitch MaxNote() { return m_nMaxPitch; }

    void SaveSettings();


private:
    void LoadSettings();


    wxString    m_sSection;         // section name to save the constrains

    bool    m_fIntervalAllowed[lmNUM_INTVALS];     //interval n allowed (0..24)
    int     m_nMinPitch;                // interval allowed range: min. diatonic pitch
    int     m_nMaxPitch;                    //      max diatonic pitch
    bool    m_fTypeAllowed[3];          // intervals: allowed types: 
                                            //      0-harmonic
                                            //      1-melodic ascending
                                            //      2-melodic descending
    bool                m_fOnlyNatural; //use only natural intervals of the scale
    lmKeyConstrains     m_oValidKeys;   //allowed key signatures

    //for interval comparison exercises
    bool    m_fFirstEqual;     // first note equal in both intervals


};


#endif  // __EARINTERVALSCONSTRAINS_H__