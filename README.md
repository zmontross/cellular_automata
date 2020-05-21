# cellular-automata

## Explanation
- The purpose of this project was to serve as an excuse for using C code while building something fairly well-known (to limit scope), visual, and interactive. I just happened to stumble-upon Raylib 3.0 when watching videos on YouTube from OneLoneCoder (javidx9) and Gamefromscratch. This framework was exactly what I was looking for. It's incredibly simple; all of the code is laid bare. It can be cross-compiled for many different platforms. There are a plethora of online examples (and I THRIVE on examples; they're how I survived my undergraduate degree), and these examples are actual demonstrations with the very simple code powering them.

- This project is really just a remake of Conway's Game of Life by a different name. I will be the first to say that this code his "Hot Garbage:tm:". I jumped-into this project with almost zero pre-planning. I visited a number of websites to confirm the basic rules and compare my code against "the solution online".

- Online resources I used for this project include:
  - The official Discord server for Raylib (which is incredibly active).
  - https://www.raylib.com/examples.html
  - https://raylib.handmade.network/
  - https://bitstorm.org/gameoflife/
  - https://www.conwaylife.com/wiki/Main_Page
  - http://pi.math.cornell.edu/~lipa/mec/lesson6.html
  - https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life


## Thoughts / Confessions

- Allowing for the resolution to be changed from the "gameplay" screen was a late addition Unfortunately this addition meant that many of the structs in the scope of the game had to be relocated. This was, unfortunately, a lot of work. Inevitably I took the lazy way out of recalculating various GUI object sizes every single game loop. Ideally everything should have been recalculated only once following the resolution/fullscreen updates. This would have been fairly trivial if I had started with any sort of plan.

- The concept of a "tool" changed quite a bit as ideas for previews or world-state-updates vs. on-screen previews changed. I'm proud to say that, although a bit laborious in creation, my initial idea for creating a tool-agnostic system for implementing new pre-arranged sets of living/dead tiles worked exactly as intended on the first try. It was flexible enough for me to add new tools at my leisure.

- Due to the aforementioned GUI-resizing problems there are more than a few items of code that could/should be removed. Part of this is is simply organizational. My single "screen_gameplay.c" file grew and grew to the point of me adding-in ASCII art to make navigation via the Visual Studio Code preview-window easier. This isn't itself too terible for personal code (obviously not for a production environment), but it was indicative that I should have been separating code into more files. I did eventually create "gameplay_typedefs.h" for containing struct types, implementations of those structs, and some utility functions. However even then both files were much too large. The name "typedefs" should have meant that the file was ONLY custom type definitions with other instantiations of those types or functions or such more appropriately being located in other files. Everything does function as-is, but most of engineering is planning and organization. I created as many problems as I'd solved by not breaking-apart my code.

- This was a purely "jump into it, get dirty, have some fun, and see what I can do" exercise (hence the lack of pre-planning). Apart from planning I should have been using git or some alternative for routine code check-ins. Although this only burned me in very minor ways throughout development I did resort to keeping snippets as comment blocks "so I didn't lose them." Source code control and repositories are the obvious solution to this basic problem.

- I probably went a bit too crazy with typedef's. This was partially driven by a desire to "flex the muscles" and get used to working with that. My last C experience truly came from Crestron SIMPL+ programming which I'd describe as "C, but no pointers, and you're breathing tear gas the whole time". As much as modern programmers in modern languages puke at the thought of manual memory management and pointers I, having had to do without either C's quirks OR the newer advantages from more recently advents in programming, welcome such things with open arms.

- Due to the nature of the engine/library I chose to use, Raylib, there is only so much I can do in minimizing the line count of my various loops. I did try to consolidate my updates to a single pass through the game grid, and even if it was a late edition (the resolutions problem) I did try, for a time, to minimize the number of repeatedly-calculated coordinates of things. In the end I just wanted it done, sped past good practice, and ignored this.

- Initially I wanted the grid to be rendered with random tile colors. Not completely random, but rather slight deviations from a starting color (such as gray +/- 'X'). At first this was acceptable, but eventually I noticed my framerate dropping significantly. It's a bit surreal seeing my framerate drop in a game consisting of non-textured squares; I have an NVidia GeForce GTX 1080 FE graphics card after all! I believe it's because I'm not really using the GPU much at all. Without any textures to give to it I'm not exactly taking advantage of what a GPU can provide in terms of resources. I wasn't satisfied with the size of the grid, and when I wanted a larger tile grid I decided to just stop rendering the non-alive tiles. This "fixed" the problem, but introduced the new problem of users having absolutely no relative cues about movement in the world... At least until a tile was set to a live state. I added a halo of lines around the cursor to visually tell the user that there was something there, and for the most part this worked pretty nicely. I already had the edge-of-grid border so outer boundaries weren't an issue. Still, I would have liked to do more, visually-speaking with the world. Maybe an odd dark-purple tile or some sparkling effect here or there in some non-regular pattern. I also considered the idea of making the alive tiles a bit nicer than just a plain white square (e.g. add shading to one edge), but I decided to leave all of this to a later day when the core of the project was complete.

- Finishing the project caused me to skip a few potential features. These included:
  - an explanation screen,
  - a credits screen (I did compromise with the startup Raylib animated logo pulled from a Raylib example),
  - detection of a "stable" grid through some form of pattern-detection algorithm and successive values of the "currently-alive tiles" counter,
  - definition/shading on the alive tiles,
  - some form of visual differentiation on the grid to make dead tiles less identical (bland) and more interesting,
  - a highlight around the alive tiles similar to the cursor highlight (experimented with it but scrapped it),
  - modifications to the grid-rendering process whereby larger rectangles would be drawn to cover the space of multiple ordinally-adjacent tiles so as to reduce the number of DrawRectangle() calls,
  - allowing for custom tool shapes to be saved-to / loaded-from files (the current system could be quickly adapted for this without any real difficulty),
  - some sort of simulation in the background of the title screen (something more interesting and relevant than plain gray to visually tell the user something of what to expect),
  - remappable keybindings using an array of function pointers,
  - "two-player-mode" as is described online for Conways's Game of Life in which multiple colors of tiles are allowed,
  - customizable grid size through some form of "world settings" screen,
  - "holes" in the grid that could be placed by players for experimentation,
  - "place large array of tool X" functionality for manual use or world initialization