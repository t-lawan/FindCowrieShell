//
//  Cell.cpp
//  FindCowrieShell
//
//  Created by Thomas Lawanson on 02/04/2021.
//

#include "Cell.h"


Cell::Cell(int _x,  int _y, int _width, int _height){
    x = _x;
    y = _y;
    width = _width;
    height = _height;
    isOccupied = false;
}

bool Cell::isWithinBounds(ofVec2f position) {
    if(position.x > x &&
       position.x < x + width &&
       position.y > y &&
       position.y < y + height){
        
        return true;
    }
    return false;
}

void Cell::setIsOccupied(bool _isOccupied) {
    isOccupied = _isOccupied;
}
