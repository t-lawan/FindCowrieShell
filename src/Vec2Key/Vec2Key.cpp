//
//  Vec2Key.cpp
//  FindCowrieShell
//
//  Created by Thomas Lawanson on 02/04/2021.
//

#include "Vec2Key.h"

Vec2Key::Vec2Key( float xValue, float yValue ){
    x = xValue;
    y = yValue;
}

ofVec2f Vec2Key::getOfVec2f() {
    return ofVec2f(x, y);
}
