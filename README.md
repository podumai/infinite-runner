<h1 align="center">Infinite-Runner</h1>
<h2 align="left">Build</h2>
<p>
  To build the application you need to install CMake and C/C++ compiler.<br>
  You can do it by building them from source or using your package manager.<br>
  For more information refer to:<br>
  <ul>
    <li>https://cmake.org/download/</li>
    <li>https://clang.llvm.org/get_started.html</li>
    <li>https://gcc.gnu.org/install/download.html</li>
  </ul>
  Also you need to get the raylib and raygui library before using further commands.<br>
  The raygui.h file that need to be compiled already located in include folder of the project.<br>
  For more information visit:<br>
  <ul>
    <li>https://github.com/raysan5/raylib</li>
    <li>https://github.com/raysan5/raygui</li>
  </ul>
  After this steps you can do the following instructions to start using the game:
  <div class="box">
    <pre><code>mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cd ..
cmake --build build</code></pre>
  </div>
  And execute it like following: <code>./build/INFINITE-RUNNER</code>
</p>
