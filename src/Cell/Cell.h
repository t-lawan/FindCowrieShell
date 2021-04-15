//
//  Cell.h
//  FindCowrieShell
//
//  Created by Thomas Lawanson on 02/04/2021.
//

#ifndef Cell_h
#define Cell_h

#include <stdio.h>
#include "ofMain.h"
class Cell {
    public:
        float x;
        float y;
        float width;
        float height;
        bool isOccupied;
        
        Cell(int _x = 0,  int _y = 0, int _width = 0, int _height = 0);
        bool isWithinBounds(ofVec2f position);
        void setIsOccupied(bool _isOccupied);

};
#endif /* Cell_h */
