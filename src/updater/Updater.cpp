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
/*! @file Updater.cpp
    @brief Implementation file for class lmUpdater
    @ingroup updates_management
*/
#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "Updater.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include <wx/dialup.h>
#include "wx/wfstream.h"
#include "wx/protocol/http.h"
#include "wx/mstream.h"         //to use files in memory
#include <wx/mimetype.h> // mimetype support
#include <wx/url.h>



#include "Updater.h"
#include "../app/AboutDialog.h"       // to get this version string
#include "../app/ErrorDlg.h"

//access to error's logger
#include "../app/Logger.h"
extern lmLogger* g_pLogger;

//access to user preferences
#include "../app/Preferences.h"

// access to global external variables
extern bool g_fReleaseVersion;          // in TheApp.cpp
extern bool g_fReleaseBehaviour;        // in TheApp.cpp


#include "UpdaterDlg.h"


//-------------------------------------------------------------------------------------------
// lmUpdater implementation
//-------------------------------------------------------------------------------------------

lmUpdater::lmUpdater()
{
    //initializations
    //m_pThread = (lmDownloadThread*)NULL;


}

lmUpdater::~lmUpdater()
{
    //if (m_pThread) {
    //    m_pThread->Delete();
    //    m_pThread->Wait();
    //    delete m_pThread;
    //    m_pThread = (lmDownloadThread*)NULL;
    //}

}

void lmUpdater::LoadUserPreferences()
{
    //load settings form user congiguration data or default values
    g_pPrefs->Read(_T("/Internet/CheckForUpdates"), &m_fCheckForUpdates, true );

}

void lmUpdater::SaveUserPreferences()
{
    //save settings in user congiguration data
    g_pPrefs->Write(_T("/Internet/CheckForUpdates"), m_fCheckForUpdates );   

}


//true if errors in conection or update information download
bool lmUpdater::DoCheck(wxString sPlatform, bool fSilent)
{
    m_sPlatform = sPlatform;

    wxXmlDocument oDoc;

    //for developing and testing do not connect to internet (it implies firewall
    //rules modification each tiem I recompile) unless explicitly selected 'Release
    //behaviour' in Debug menu.
    if (g_fReleaseVersion || g_fReleaseBehaviour)
    {
        //Release behaviour. Access to internet

        //verify if internet is available
        if (!CheckInternetConnection()) {
            if (!fSilent) {
                lmErrorDlg dlg(m_pParent, _("Error"), _("You are not connected to internet!\n\n \
To check for updates LenMus needs internet connection. \n \
Please, connect to internet and then retry."));
                dlg.ShowModal();
            }
            return true;
        }

        // ensure we can build a wxURL from the given URL
        wxString sUrl = _T("http://www.lenmus.org");
        wxURL oURL(sUrl);
        wxASSERT( oURL.GetError() == wxURL_NOERR);

        //Setup user-agent string to be identified not as a bot but as a browser
        wxProtocol& oProt = oURL.GetProtocol();
        wxHTTP* pHTTP = (wxHTTP*)&oProt;
        pHTTP->SetHeader(_T("User-Agent"), _T("LenMus Phonascus updater"));
        oProt.SetTimeout(90);              // 90 sec

        //create the input stream by establishing http connection
        wxInputStream* pInput = oURL.GetInputStream();
        if (!pInput) {
            // something is wrong with the input URL...
            if (!fSilent)
                wxMessageBox(_("Something is wrong with the input URL: ") + sUrl);
            return true;
        }
        if (!pInput->IsOk()) {
            delete pInput;
            if (!fSilent) {
                lmErrorDlg dlg(m_pParent, _("Error"), _("Connection with the server could \
not be established. \
Check that you are connected to the internet and that no firewalls are blocking \
this program; then try again. If problems continue, the server \
may be down. Please, try again later."));
                dlg.ShowModal();
            }
            return true;
        }

        //download updates data file and analyze it
        if (!oDoc.Load(*pInput)) {
            g_pLogger->ReportProblem(_("Error loading XML file "));
            return true;
        }

    }
    else {
        //Debug behaviour. Instead of accesing Internet use local files
        wxString sFilename = _T("c:\\usr\\desarrollo_wx\\lenmus\\docs\\src\\UpdateData.xml");
        if (!oDoc.Load(sFilename) ) {
            g_pLogger->ReportProblem(_("Error loading XML file "));
            return true;
        }
    }

    //Verify type of document. Must be <UpdateData>
    wxXmlNode *pRoot = oDoc.GetRoot();
    if (pRoot->GetName() != _T("UpdateData")) {
        g_pLogger->ReportProblem(
            _("Error. <%s> files are not supported"), pRoot->GetName() );
        return true;
    }
    
    //analyze document and extract information for this platform
    ParseDocument(pRoot);

    m_fNeedsUpdate = (m_sVersion != _T("") && m_sVersion != LM_VERSION_STR);

    return false;       //no errors

}

bool lmUpdater::CheckInternetConnection()
{
    bool fConnected = false;    //assume not connected

    // check connection
    wxDialUpManager* pManager = wxDialUpManager::Create();
    if (pManager->IsOk()) {
        fConnected = pManager->IsOnline();
    }

    delete pManager;
    return fConnected;
}


void lmUpdater::ParseDocument(wxXmlNode* pNode)
{
    //initialize results
    m_sVersion = _T("");
    m_sDescription = _T("");
    m_sPackage = _T("No package!");

    //start parsing. Loop to find <platform> tag for this platform
    g_pLogger->LogTrace(_T("lmUpdater"),
        _T("[lmUpdater::ParseDocument] Starting the parser"));
    pNode = GetFirstChild(pNode);
    wxXmlNode* pElement = pNode;
    
    while (pElement) {
        if (pElement->GetName() != _T("platform")) {
            g_pLogger->LogError(
                _T("[lmUpdater::ParseDocument] Expected tag <platform> but found <%s>"),
                pElement->GetName() );
            return;
        }
        else {
            //if this platform found analyze it and finish
            if (GetAttribute(pElement, _T("name"), _T("")) == m_sPlatform) {
                g_pLogger->LogTrace(_T("lmUpdater"),
                    _T("[lmUpdater::ParseDocument] Analyzing data for <platform name='%s'>"),
                    m_sPlatform );

                //find first child
                pNode = GetFirstChild(pNode);
                pElement = pNode;
                  
                while (pElement) {
                    if (pElement->GetName() == _T("version")) {
                        m_sVersion = GetText(pElement);
                    }
                    else if (pElement->GetName() == _T("package")) {
                        m_sPackage = GetText(pElement);
                    }
                    else if (pElement->GetName() == _T("description")) {
                        m_sDescription = GetText(pElement);
                    }
                    else if (pElement->GetName() == _T("download_url")) {
                        m_sUrl = GetText(pElement);
                    }
                    else {
                        g_pLogger->LogError(
                            _T("[lmUpdater::ParseDocument] Expected tag <version> or <description> but found <%s>"),
                            pElement->GetName() );
                    }

                    // Find next sibling 
                    pNode = GetNextSibling(pNode);
                    pElement = pNode;
                }
                return;     //done
            }
            //it is not this platform. Continue with next <platform> tag
        }

        // Find next next <platform> tag
        pNode = GetNextSibling(pNode);
        pElement = pNode;
    
    }

}

void lmUpdater::CheckForUpdates(wxFrame* pParent, bool fSilent)
{
    if (!m_fCheckForUpdates) return;

    //initializations
    m_pParent = pParent;
    m_fNeedsUpdate = false;
    
    //Inform user and ask permision to proceed
    if (!fSilent) {
        lmUpdaterDlgStart dlgStart(m_pParent);
        if (dlgStart.ShowModal() == wxID_CANCEL) return;  //updater canceled
    }

    //conect to server and get information about updates
    if (DoCheck(_T("Win32"), fSilent)) {
        //Error. Not posible to do check
        return;
    }

    //inform about results
    if (!m_fNeedsUpdate) {
        //no updates available
        if (!fSilent) wxMessageBox(_T("No updates available."));
    }
    else {
        //update available. Create and show informative dialog
        lmUpdaterDlgInfo dlg(m_pParent, this);
        dlg.AddPackage(GetPackage(), GetDescription());
        dlg.ShowModal();
    }

}


bool lmUpdater::DownloadFiles()
{
/*! @todo   Function ::wxLaunchDefaultBrowser() does not work. So I have defined
    LaunchDefaultBrowser() and copied the latest code for wxLaunchDefaultBrowser
    taken from wxWidgets CVS. Remove this code at next wxWidgets release,
    once checked that wxLaunchDefaultBrowser() works ok, and change next statement.
*/
    LaunchDefaultBrowser( m_sUrl );

    // In order to allow for download cancellation, it is going to be implemented
    // in a thread. Communication with the thread takes place by events
    // generated in the thread.
    //
    //

//    //create downloader thread
//    m_pThread = new lmDownloadThread((wxEvtHandler*)NULL);
//    if (m_pThread->Create() != wxTHREAD_NO_ERROR ||
//        m_pThread->Run() != wxTHREAD_NO_ERROR) {
//        lmErrorDlg dlg(m_pParent, _("Error"),
//            _("Low resources; cannot run the thread for downloading files.\n\n \
//Close some applications and then retry."));
//        dlg.ShowModal();
//        wxLogMessage(wxT("[lmUpdater::DoCheck] Can not run the download thread."));
//        return true;
//    }
//
//    //According to wxWidgets documentation if you want to use sockets or derived 
//    //classes such as wxFTP in a secondary thread, call wxSocketBase::Initialize()
//    //(undocumented) from the main thread before creating any sockets 
//    wxSocketBase::Initialize();
//
//
//    //start download of file
//    m_pThread->SetURL( m_sUrl );
//    m_pThread->StartDownload();
//
    return false;

}


//--------------------------------------------------------------------------------------
// Copied from XMLParser.cpp
// If you need to fix something here check if the fix is also applicable there
//--------------------------------------------------------------------------------------

// Copied from XMLParser.cpp
// If you need to fix something here check if the fix is also applicable there
wxString lmUpdater::GetText(wxXmlNode* pElement)
{
    //    Receives node of type ELEMENT and returns its text content
    wxASSERT(pElement->GetType() == wxXML_ELEMENT_NODE);

    wxXmlNode* pNode = pElement->GetChildren();
    wxString sName = pElement->GetName();
    wxString sValue = _T("");
    
    if (pNode->GetType() == wxXML_TEXT_NODE) {
        sValue = pNode->GetContent();
    }
    return sValue;
}

// Copied from XMLParser.cpp
// If you need to fix something here check if the fix is also applicable there
wxString lmUpdater::GetAttribute(wxXmlNode* pNode, wxString sName, wxString sDefault)
{
    wxXmlProperty* pAttrib = pNode->GetProperties();
    while(pAttrib) {
        if (pAttrib->GetName() == sName)
            return pAttrib->GetValue();
        pAttrib = pAttrib->GetNext();
    }

    return sDefault;
}

wxXmlNode* lmUpdater::GetNextSibling(wxXmlNode* pNode)
{
    // Return next sibling element or NULL if no more
    pNode = pNode->GetNext();
    while (pNode && pNode->GetType() != wxXML_ELEMENT_NODE)
        pNode = pNode->GetNext();
    return pNode;

}
wxXmlNode* lmUpdater::GetFirstChild(wxXmlNode* pNode)
{
    // Return first child element or NULL if no more
    pNode = pNode->GetChildren();
    while (pNode && pNode->GetType() != wxXML_ELEMENT_NODE)
        pNode = pNode->GetNext();
    return pNode;
}


/* ----------------------------------------------------------------------------
    Launch default browser
*/
/*! @todo   This is the latest (18/may/2006) code for ::wxLaunchDefaultBrowser() 
    function taken from wxWidgets CVS. Remove this code at next wxWidgets release,
    once checked that wxLaunchDefaultBrowser() works ok.
----------------------------------------------------------------------------*/

bool LaunchDefaultBrowser(const wxString& urlOrig)
{
    // set the scheme of url to http if it does not have one
    wxString url(urlOrig);
    if ( !wxURI(url).HasScheme() )
        url.Prepend(wxT("http://"));

#if defined(__WXMSW__)
    WinStruct<SHELLEXECUTEINFO> sei;
    sei.lpFile = url.c_str();
    sei.lpVerb = _T("open");
    sei.nShow = SW_SHOWNORMAL;

    ::ShellExecuteEx(&sei);

    const int nResult = (int) sei.hInstApp;

    // Firefox returns file not found for some reason, so make an exception
    // for it
    if ( nResult > 32 || nResult == SE_ERR_FNF )
    {
#ifdef __WXDEBUG__
        // Log something if SE_ERR_FNF happens
        if ( nResult == SE_ERR_FNF )
            wxLogDebug(wxT("SE_ERR_FNF from ShellExecute -- maybe FireFox?"));
#endif // __WXDEBUG__
        return true;
    }
#elif defined(__WXMAC__)
    OSStatus err;
    ICInstance inst;
    SInt32 startSel;
    SInt32 endSel;

    err = ICStart(&inst, 'STKA'); // put your app creator code here
    if (err == noErr)
    {
#if !TARGET_CARBON
        err = ICFindConfigFile(inst, 0, NULL);
#endif
        if (err == noErr)
        {
            ConstStr255Param hint = 0;
            startSel = 0;
            endSel = url.Length();
            err = ICLaunchURL(inst, hint, url.fn_str(), endSel, &startSel, &endSel);
            if (err != noErr)
                wxLogDebug(wxT("ICLaunchURL error %d"), (int) err);
        }
        ICStop(inst);
        return true;
    }
    else
    {
        wxLogDebug(wxT("ICStart error %d"), (int) err);
        return false;
    }
#elif wxUSE_MIMETYPE
    // Non-windows way
    bool ok = false;
    wxString cmd;

    wxFileType *ft = wxTheMimeTypesManager->GetFileTypeFromExtension(_T("html"));
    if ( ft )
    {
        wxString mt;
        ft->GetMimeType(&mt);

        ok = ft->GetOpenCommand(&cmd, wxFileType::MessageParameters(url));
        delete ft;
    }

    if ( !ok || cmd.empty() )
    {
        // fallback to checking for the BROWSER environment variable
        cmd = wxGetenv(wxT("BROWSER"));
        if ( !cmd.empty() )
            cmd << _T(' ') << url;
    }

    ok = ( !cmd.empty() && wxExecute(cmd) );
    if (ok)
        return ok;

    // no file type for HTML extension
    wxLogError(_T("No default application configured for HTML files."));

#endif // !wxUSE_MIMETYPE && !__WXMSW__

    wxLogSysError(_T("Failed to open URL \"%s\" in default browser."),
                  url.c_str());

    return false;
}
