# Winter-CS32-2020
Data structures and algorithms course at UCLA using C++. Several of these projects have frameworks created by David A. Smallberg and Carey Nachenberg, the CS32 professors at UCLA for this quarter, while the implementations were designed by students. In comparison to the previous course, CS31, the projects offered in this course offer much more creative flexibility in coding style and structure. This repository features the two most important projects in the class.

# Score Distribution

| Project  | Score |
| ------------- | ------------- |
| 3  | 100/100  |
| 4  | 99/100  |

# Project 3
![Imgur](https://i.imgur.com/ov4VDne.png)

Kontagion focuses on the role of Socrates, a bacteriologist shrunk down into a petri dish to fight off swarms of bacteria. (Sounds unbelievable but this was our actual project) The project focuses on implementing polymorphism to derive each of the game's characters, enemies, and sprites.

# Project 4
![Imgur](https://i.imgur.com/X7na80m.png)

Easily the most interesting project, Goober Eats tackles a commonly asked Computer Science problem. What is the least distance route that can be made given a set of points? Known as the traveling salesman problem, there have been multiple papers written on the subject. In my implementation, the A* algorithm finds the most efficient route between two points while simulated annealing finds a relative minimum for the most efficient route from a set of points. Data was read from a text file of geocoordinates and streetnames associated with the Westwood in Los Angeles. Through tests with Google Maps, my results always mirrored the expected distance +/- 5%, showing that these two sets of algorithms fit. 

**During my research into these algorithms, the following resources were helpful:** <br>
https://www.youtube.com/watch?v=-L-WgKMFuhE - Thorough explanation and visualization of A* <br>
https://www.youtube.com/watch?v=SC5CX8drAtU - Visualization of TSP and the effectiveness of simulated annealing <br>
https://pdfs.semanticscholar.org/6178/2873250a6096c098729ed3bda2a5e1a3a31a.pdf - Dense read of temperature in simulated annealing, but entirely worth it for understanding the hyperparameters of simulated annealing and optimizing your set up
