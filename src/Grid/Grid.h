//
//  Grid.hpp
//  FindCowrieShell
//
//  Created by Thomas Lawanson on 23/04/2021.
//

#ifndef Grid_hpp
#define Grid_hpp

#include <stdio.h>
#include "ofMain.h"
#include "Cell.h"

class Grid {
    public:
        int gridSize = 4;
        vector<Cell> cells;
        void setup();
        void update();
        void draw();
};
#endif /* Grid_hpp */
