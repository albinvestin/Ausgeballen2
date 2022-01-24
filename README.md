How to make on windows:
mingw32-make.exe

Useful:  
alias make=mingw32-make.exe


TODO: 
[x] Add fixed game update time, see https://gamedev.stackexchange.com/questions/19571/how-can-i-process-continuously-held-keys-with-sdl or https://fabiensanglard.net/timer_and_framerate/index.php or https://gafferongames.com/post/fix_your_timestep/

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
[x] Make bullets and players as data structures
[] Make a movement handler
[] Fix naming of playerindex (= 0, 1, 2, 3) vs player numbering (1, 2, 3 , 4) convention
[] Make own serializing framework.
[x] Fix so that host can send input shooting to client
[x] Handle framerate spikes (grabbing window)

References:
https://floating-point-gui.de/errors/comparison/
https://en.wikipedia.org/wiki/Law_of_Demeter
Copy constructor: https://www.cplusplus.com/articles/y8hv0pDG/



cereal failed to read 4 bytes from inputsteam
https://github.com/USCiLab/cereal/issues/452
When to use smart pointers:
https://stackoverflow.com/questions/7657718/when-to-use-shared-ptr-and-when-to-use-raw-pointers

Herb Sutter: https://herbsutter.com/2013/06/05/gotw-91-solution-smart-pointer-parameters/
Guideline: Don’t pass a smart pointer as a function parameter unless you want to use or manipulate the smart pointer itself, such as to share or transfer ownership.
Guideline: Prefer passing objects by value, *, or &, not by smart pointer.