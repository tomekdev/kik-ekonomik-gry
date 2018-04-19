#!/bin/bash
#i686-w64-mingw32-g++ -o kik.exe main.cpp -DANIMATIONS -Iwinsdl/include -Lwinsdl/lib -lSDL2 -lSDL2_image -lSDL2_ttf -DWINDOWS_BUILD -mconsole -lmingw32 -static-libgcc -static-libstdc++
i686-w64-mingw32-g++ -c -o AI.o AI.cpp -DANIMATIONS -Iwinsdl/include -Lwinsdl/lib -lSDL2 -lSDL2_image -lSDL2_ttf -DWINDOWS_BUILD -mconsole -lmingw32 -static-libgcc -static-libstdc++
i686-w64-mingw32-g++ -c -o drawboard.o drawboard.cpp -DANIMATIONS -Iwinsdl/include -Lwinsdl/lib -lSDL2 -lSDL2_image -lSDL2_ttf -DWINDOWS_BUILD -mconsole -lmingw32 -static-libgcc -static-libstdc++
i686-w64-mingw32-g++ -c -o game.o game.cpp -DANIMATIONS -Iwinsdl/include -Lwinsdl/lib -lSDL2 -lSDL2_image -lSDL2_ttf -DWINDOWS_BUILD -mconsole -lmingw32 -static-libgcc -static-libstdc++
i686-w64-mingw32-g++ -c -o gamelogics.o gamelogics.cpp -DANIMATIONS -Iwinsdl/include -Lwinsdl/lib -lSDL2 -lSDL2_image -lSDL2_ttf -DWINDOWS_BUILD -mconsole -lmingw32 -static-libgcc -static-libstdc++
i686-w64-mingw32-g++ -c -o graphics.o graphics.cpp -DANIMATIONS -Iwinsdl/include -Lwinsdl/lib -lSDL2 -lSDL2_image -lSDL2_ttf -DWINDOWS_BUILD -mconsole -lmingw32 -static-libgcc -static-libstdc++
i686-w64-mingw32-g++ -c -o init.o init.cpp -DANIMATIONS -Iwinsdl/include -Lwinsdl/lib -lSDL2 -lSDL2_image -lSDL2_ttf -DWINDOWS_BUILD -mconsole -lmingw32 -static-libgcc -static-libstdc++
i686-w64-mingw32-g++ -c -o main.o main.cpp -DANIMATIONS -Iwinsdl/include -Lwinsdl/lib -lSDL2 -lSDL2_image -lSDL2_ttf -DWINDOWS_BUILD -mconsole -lmingw32 -static-libgcc -static-libstdc++
i686-w64-mingw32-g++ -c -o score.o score.cpp -DANIMATIONS -Iwinsdl/include -Lwinsdl/lib -lSDL2 -lSDL2_image -lSDL2_ttf -DWINDOWS_BUILD -mconsole -lmingw32 -static-libgcc -static-libstdc++
i686-w64-mingw32-g++ -c -o texture.o texture.cpp -DANIMATIONS -Iwinsdl/include -Lwinsdl/lib -lSDL2 -lSDL2_image -lSDL2_ttf -DWINDOWS_BUILD -mconsole -lmingw32 -static-libgcc -static-libstdc++
i686-w64-mingw32-g++ -o kik.exe texture.o AI.o drawboard.o game.o gamelogics.o graphics.o init.o main.o score.o -DANIMATIONS -Iwinsdl/include -Lwinsdl/lib -lSDL2 -lSDL2_image -lSDL2_ttf -DWINDOWS_BUILD -mconsole -lmingw32 -static-libgcc -static-libstdc++

mv -v kik.exe winbuild/
cp -rv data winbuild/
tar -cv winbuild > winbuild.tar
