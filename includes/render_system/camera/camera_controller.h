#pragma once
#include <wx/event.h>

class CameraController {
public:
    virtual void onKeyPressed(wxKeyEvent& event) = 0;
    virtual void onMouseEvent(wxMouseEvent& event) = 0;
};