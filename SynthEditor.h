//
//  SynthEditor.h
//  TestSynthAU
//
//  Used to specify the contents and layout of the TestSynthAU User Interface (UI).
//

#ifndef __SynthEditor_h__
#define __SynthEditor_h__

enum CONTROL_TYPE
{
    ROTARY, // rotary knob (pot)
    BUTTON, // push button (trigger)
    TOGGLE, // on/off switch (toggle)
    SLIDER, // linear slider (fader)
    MENU,   // drop-down list (menu)
};

typedef Rectangle<int> Bounds;

struct Control
{
    String name;            // name for control label / saved parameter
    int parameter;          // parameter index associated with control
    CONTROL_TYPE type;      // control type (see above)
    
    // ROTARY and SLIDER only:
    float min;              // minimum slider value (e.g. 0.0)
    float max;              // maximum slider value (e.g. 1.0)
    
    float initial;          // initial value for slider (e.g. 0.0)
    
    Bounds size;            // position (x,y) and size (height, width) of the control (use AUTO_SIZE for automatic layout)
    
    const char* const options[8]; // text options for menus and group buttons
};

const Bounds AUTO_SIZE = Bounds(-1,-1,-1,-1); // used to trigger automatic layout
enum { kParam0, kParam1, kParam2, kParam3, kParam4, kParam5, kParam6, kParam7, kParam8, kParam9 };

//=========================================================================
// UI_CONTROLS - Use this array to completely specify your UI
// - tell the system what controls you want
// - add or remove controls by adding or taking away from the list
// - each control is linked to the specified parameter name and identifier
// - controls can be of different types - rotary, button, toggle, slider (see above)
// - for rotary and linear sliders, you can set the range of values
// - by default, the controls are laid out in a grid, but you can also move and size them manually
//   i.e. replace AUTO_SIZE with Bounds(50,50,100,100) to place a 100x100 control at (50,50)

#define RESTRICTED(name, id, size) \
{   name,       id,         ROTARY, 0.0, 1.0, 0.0,    size        }


const auto sliderSize = 60;
const auto sliderX = 30;
const auto sliderX2 = 320;
const auto menuHeight = 40;
const auto menuWidth = 170;
const auto menuX = 130;

const Control UI_CONTROLS[] = {
    
    {   "Consonant", kParam0, MENU, 0.0, 1.0, 0.0, { menuX, 30, (menuWidth / 2), menuHeight }, { "B", "D", "F", "P", "T", "V", "Z" } },
    {   "Vowel 1",   kParam0, MENU, 0.0, 1.0, 0.0, { menuX, 110, menuWidth, menuHeight }, { "eee", "ihh", "aaa", "ooo" } },
    {   "Vowel 2",   kParam1, MENU, 0.0, 4.0, 2.0, { menuX, 185, menuWidth, menuHeight }, { "eee", "ihh", "aaa", "ooo" } },
    {   "Vowel Transition Time",  kParam2,    ROTARY, 0.1, 2.0, 1.0, { sliderX, 65, sliderSize, sliderSize } },
    {   "Fade Out Time",          kParam3,    ROTARY, 0.1, 4.0, 4.0, { sliderX, 150, sliderSize, sliderSize } },
    
    {   "Meter",  kParam4,    ROTARY, 0.0, 1.0, 0.0, { sliderX2, 15, sliderSize, sliderSize } },
    {   "Meter",  kParam4,    ROTARY, 0.0, 1.0, 0.0, { sliderX2, 95, sliderSize, sliderSize } },
    {   "Meter",  kParam5,    ROTARY, 0.0, 1.0, 0.0, { sliderX2, 175, sliderSize, sliderSize } },
    
    {   "Cutoff", kParam6, ROTARY, 500.0, 20000.0, 10000.0, {(menuX + 105), 25, sliderSize, sliderSize} },
};

const int kNumberOfControls = sizeof(UI_CONTROLS) / sizeof(Control);
const int kNumberOfParameters = kNumberOfControls;

#endif
