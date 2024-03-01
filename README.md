Line Follower Maze
An Arduino micro controller based robot which first analyzes the maze in the dry run by following lines through IR sensors and then calculates the shortest path from the source to the destination.
In this paper we have discussed a unique general algorithm for exploring and solving any kind of line maze with another simple one for simple mazes without loops or with loops having highest two branches none of which are inward. For the general algorithm, we need a method to map the whole maze, which is required if the maze is complex. The proposed maze mapping system is based on coordinate system and after mapping the . In order to find the coordinates of the turning points and junctions, linear distances between the points are needed, for which wheel encoder was used. However, due to non-linear movement of robot, the directly measured distance from the encoder has some error and to remove this error an idea is built up which ended by deriving equations that give us almost exact linear distance between two points from the reading of wheel encoder of the robot moving in a non-linear path.

Table of Contents
1. Problem Statement
2. Installation/Integeration
Problem Statement
Teams have to build an autonomous robot which can follow a white line and keep track of directions while going through the maze.
 The bot has to analyze the path in the dry run and has togo through the mesh from the starting point to the ending point in minimum possible time.
ARENA
The game field consists of an arena having dimensions 2310 mm X 1980 mm (lxb).It consists of the following:
1.The arena is composed of random paths made up of white strips.
2.Angle between two adjacent white lines in the path is either 90 deg or 135 deg.
3.The width of all white stripes will be 30mm.
4.The actual arena at the competitionwill consist of slight alterations in the path.

Installation/Integeration
Requirement, wires , Breadboard, 8 IR sensor array board, Lithium-ion polymer battery, Arduino software, Wheels, L293D Motor Driver, 7805


Installation
Just Copy and paste the above given code in MazeSolverRobot Directory -> ArduinoCode_Final.ino in the new project of Arduino Software. Save and Upload the Code in the Arduino Board and Design a map with Black lines on White Background.

Note : This Code is valid for black lines on white background. Just apply "!" NOT operater in all the if conditions to reverse the color combination
You are All set to go :)
Author : Tapiwa Manhando
