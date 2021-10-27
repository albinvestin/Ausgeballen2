How to make on windows:
mingw32-make.exe

Useful:  
alias make=mingw32-make.exe


TODO: 
[] Add fixed game update time, see https://gamedev.stackexchange.com/questions/19571/how-can-i-process-continuously-held-keys-with-sdl

[x] make player model
[x] shoot bullet
[x] delete bullets that hit edge
[x] make recoil from shooting
[x] make bullet collision with other players
([] Take care of that the position is top left corner and collision is using radius., No issue when players are the same size?)
[x] make player-player collision
[x] add score
[] Read all of this: https://www.cplusplus.com/articles/y8hv0pDG/
[] Modify members of other classes and passing them by reference is bad practice, either use smart pointers for this or could this be structured in another way?
[] Add check so that players are moving from eachother so that they do not get stuck in eachother. Or is this a feature? >:D
[] offset the map to middle of screen

References:
https://floating-point-gui.de/errors/comparison/
