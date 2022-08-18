#include "widgets/app.h"
#include "widgets/mainwindow.h"
#include <wx/image.h>
#include <wx/imagjpeg.h>
#include <wx/imagpng.h>
#include <wx/imagtga.h>


bool Application::OnInit() {
    wxImage::AddHandler(new wxPNGHandler());
    wxImage::AddHandler(new wxTGAHandler());
    wxImage::AddHandler(new wxJPEGHandler());
    
    auto mainWindow = new MainWindow();
    mainWindow->Show(true);
    return true;
}