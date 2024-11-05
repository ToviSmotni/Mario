# Mario Game in C++ using OpenCV 
This is a basic implementation of the classic Mario game using C++ and OpenCV, <br/>
with design patterns such as Observable, Factory, Entities Pool, and Animation Graphics. <br/>
It also has physics implemented so that Mario can run right, run left, jump, and duck. <br/>

# Dependencies
OpenCV (version 4.5.2 or later) <br/>
[download OpenCV](https://opencv.org/releases/) <br/>
and place them under OpenCV_451/bin folder <br/>
C++11 or later <br/>
# Installation <br/>
Clone the repository <br/>
Make sure you have OpenCV installed <br/>
Build the project using a C++ compiler <br/>
# How to Play 
Run the executable file <br/>
Use the following keys to control Mario: <br/>
'w' key : jump <br/>
's' key: duck <br/>
'a' key : move left <br/>
'd' key: move right <br/>
Collect coins and power-ups while avoiding enemies <br/>

Design Patterns Used
# Observable
The Observable pattern is used to notify the game engine of events such as collisions and user input. <br/>
Classes such as Mario and Enemy inherit from the Observable class, which contains methods for registering and notifying observers. <br/>

# Factory
The Factory pattern is used to create game entities such as Mario, Enemy, and PowerUp. <br/>
The EntityFactory class is responsible for creating these entities based on user input and game events. <br/>

# Entities Pool
The Entities Pool pattern is used to reuse game entities such as Enemy and PowerUp to minimize memory allocation and deallocation. <br/>
The EntitiesPool class manages a pool of entities that can be reused when they are no longer needed. <br/>

#Animation Graphics
Animation Graphics are used to create the animations of characters and objects in the game. <br/>
The Animation class is responsible for loading and animating sprites for game entities. <br/>

Physics
Physics are implemented using a basic kinematic model to simulate the movement of objects in the game. <br/>
The Physics class contains methods for handling collisions, gravity, and movement. <br/>

# Contributions
Contributions to this project are welcome. Please fork the repository and submit a pull request for review.

# License
This project is licensed under the MIT License. See the LICENSE file for more information.

