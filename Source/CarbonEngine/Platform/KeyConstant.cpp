/*
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "CarbonEngine/Common.h"
#include "CarbonEngine/Platform/KeyConstant.h"

namespace Carbon
{

const auto keyNames = std::array<String, KeyLast>{
    {"None", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
     "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9",
     "F10", "F11", "F12", "Up arrow", "Down arrow", "Left arrow", "Right arrow", "Insert", "Delete", "Home", "End", "Page up",
     "Page down", "Plus", "Minus", "Equals", "Backspace", "Left bracket", "Right bracket", "Enter", "Semicolon", "Apostrophe",
     "Comma", "Period", "Forward slash", "Backslash", "Star", "Escape", "Grave accent", "Caps lock", "Tab", "Left alt",
     "Right alt", "Left control", "Right control", "Left shift", "Right shift", "Left meta", "Right meta", "Spacebar", "Start",
     "Select", "Square", "Cross", "Circle", "Triangle", "Move", "Left button 1", "Left button 2", "Left button 3",
     "Left button 4", "Left button 5", "Right button 1", "Right button 2", "Right button 3", "Right button 4", "Right button 5",
     "Numpad 0", "Numpad 1", "Numpad 2", "Numpad 3", "Numpad 4", "Numpad 5", "Numpad 6", "Numpad 7", "Numpad 8", "Numpad 9",
     "Numpad plus", "Numpad minus", "Numpad equals", "Numpad enter", "Numpad comma", "Numpad period", "Numpad forward slash",
     "Numpad star", "Kanji"}};

CARBON_API const String& getKeyConstantAsString(KeyConstant key)
{
    return key >= KeyNone && key < KeyLast ? keyNames[key] : String::Empty;
}

}
