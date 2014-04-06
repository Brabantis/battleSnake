//
//  Main.cpp
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 12/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

//==================================================//
//                                                  //
//          Main function and game loop             //
//                                                  //
//==================================================//

#include <iostream>
#include <string>
#include <vector>

#include "Spaceship.h"
#include "FleetMember.h"
#include "Graphics.h"
#include "Engine.h"

// I'm including many more headers than I need, I will eliminate the superfluous ones later

// TODO: basically every function in Engine, using the level, graphics for battle, attacks of the characters. A graphical fleet builder.

using namespace std;

int main()
{
    Engine engi;
    engi.start();
	return 0;
}