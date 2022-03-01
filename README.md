# Pacman

A pacman game for Data Structure and Algorithm Project
Various ghosts Ele, Meep, Timid, Helper chase Pacman with the power of path finding algorithms like **Depth First Search**, **Breadth First Search** and **A Star**.
Can you dodge all of them and collect all the points ?

**Meep**: Isn't the brightest, will just find a path regardless if it is the shortest, but you bet he is gonna chase you

**Ele**: Calm And Collected, Makes Calculated Decisions using AStar

**Timid**: A smart fellow, but gets easily flustered.

**Helper**: Cuts off the path.

## Dependencies

- SDL2
- SDL2_image
- SDL2_mixer
- SDL2_ttf

## Build Instructions

### Arch Linux / Ubuntu

- Clone the repo.
- Run the build.sh. Should just work

### Windows

- Download [SDL2](https://www.libsdl.org/download-2.0.php),[SDL2_image](https://www.libsdl.org/projects/SDL_image/),[SDL2_mixer](https://www.libsdl.org/projects/SDL_mixer/),[SDL2_ttf](https://www.dll4free.com/sdl2_ttf.dll.html) 
- Extract and store in a folder
- Open  ``Pacman.sln``  
- Open project properties
- Set **Project Configuration** to **All configuration**
- Set **Platform** to **Active(x64)**
- In **General** for *C/C++*
  - Set the **Additional include directory** to *(Local Disk):\(SDL directory name)\include*
- Open **General** for *Linker*
  - Set the  **Additional Library Directory** to *(Local Disk):\(SDL directory name)\lib*
- Open **Input** for *Linker*
  - Set the **Additional Dependencies** to ``SDL2_image.lib,SDL2_ttf.lib,SDL2_mixer.lib,SDL2.lib,SDL2main.lib``
- Now Run the project














P.S. Thank you Donkey Kong and Legend of Zelda for the music.
