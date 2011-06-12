//---------------------------------------------------------------------------------------
//    LenMus Phonascus: The teacher of music
//    Copyright (c) 2002-2011 LenMus project
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
//---------------------------------------------------------------------------------------

#ifndef __LENMUS_SCORE_CANVAS_H__        //to avoid nested includes
#define __LENMUS_SCORE_CANVAS_H__

//lenmus headers
#include "lenmus_canvas.h"

#include "lenmus_injectors.h"

//wxWidgets headers
#include "wx/wxprec.h"
#include <wx/app.h>
#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/filedlg.h>
#include <wx/image.h>
#include <wx/dc.h>
#include <wx/dcmemory.h>
#include <wx/event.h>
#include <wx/aui/aui.h>

//lomse headers
#include "lomse_doorway.h"
#include "lomse_document.h"
#include "lomse_graphic_view.h"
#include "lomse_interactor.h"
#include "lomse_presenter.h"
#include "lomse_events.h"
#include "lomse_internal_model.h"
#include "lomse_analyser.h"

//other headers
#include <iostream>
#include <UnitTest++.h>


using namespace lomse;


namespace lenmus
{

//---------------------------------------------------------------------------------------
// ScoreCanvas is a window on which we show the scores
class ScoreCanvas: public Canvas
{
public:
    ScoreCanvas(ContentFrame *parent, ApplicationScope& appScope, LomseDoorway& lomse);
    virtual ~ScoreCanvas();

    //callback wrappers
    static void wrapper_force_redraw(void* pThis);
    static void wrapper_update_window(void* pThis);
    static void wrapper_on_lomse_event(void* pThis, EventInfo& event);

    //commands from main frame
    void display_document(std::string& filename);
    void zoom_in();
    void zoom_out();
    void on_key(int x, int y, unsigned key, unsigned flags);
    void force_redraw();
    void update_window();
    void on_document_updated();
    void start_play();

    void open_test_document();
    void update_view_content();
    void on_key_event(wxKeyEvent& event);

protected:
    // event handlers
    void on_paint(wxPaintEvent& event);
    void on_size(wxSizeEvent& event);
    void on_mouse_event(wxMouseEvent& event);

private:
    ApplicationScope& m_appScope;

    // In this example we are going to display an score on a canvas window.
    // Let's define the necessary variables:
    LomseDoorway&   m_lomse;        //the Lomse library doorway
    Presenter*      m_pPresenter;
    Interactor*     m_pInteractor;  //to interact with the View
    Document*       m_pDoc;         //the score to display

    //the Lomse View renders its content on a bitmap. To manage it, Lomse
    //associates the bitmap to a RenderingBuffer object.
    //It is your responsibility to render the bitmap on a window.
    //Here you define the rendering buffer and its associated bitmap to be
    //used by the previously defined View.
    RenderingBuffer     m_rbuf_window;
    wxImage*            m_buffer;               //the image to serve as buffer
    unsigned char*      m_pdata;                //ptr to the real bytes buffer
    int                 m_nBufWidth, m_nBufHeight;      //size of the bitmap

    //some additinal variables
    bool    m_view_needs_redraw;      //to control when the View must be re-drawed


    void on_lomse_event(EventInfo& event);

    void delete_rendering_buffer();
    void create_rendering_buffer(int width, int height);
    void do_update_window(wxDC& dc);
    void update_rendering_buffer();

    unsigned get_keyboard_flags(wxKeyEvent& event);
    unsigned get_mouse_flags(wxMouseEvent& event);
    void reset_boxes_to_draw();


    DECLARE_EVENT_TABLE()
};


}   // namespace lenmus

#endif    // __LENMUS_SCORE_CANVAS_H__

