# Moer_ScenePreviewer
Moer_ScenePreviewer is a software rasterizer designed to offer fast scene previews for [Moer](https://github.com/NJUCG/Moer).

See [Moer](https://github.com/NJUCG/Moer) for more information about the scene format.

# Build
Moer_ScenePreviewer uses [xmake](https://github.com/xmake-io/xmake) for building. You need to have xmake installed.
```
$ xmake
```

# Run
```
$ xmake r Moer_ScenePreviewer "Scene Folder Path"
```
For example:
```
$ xmake r Moer_ScenePreviewer "..\\..\\..\\..\\scenes\\teapot\\"
```
# Branches
The main branch uses OpenGL for rendering.

If you want to check the CPU rendering version of the code, please switch to the cpu-rendering branch.

# TODO List
- [ ] ImGui for creating GUI
- [ ] Camera movement

# Gallery
[![teapot.md.png](https://z4a.net/images/2023/12/08/teapot.md.png)](https://z4a.net/image/jyQLYk)
[![green-bathroom.md.png](https://z4a.net/images/2023/12/08/green-bathroom.md.png)](https://z4a.net/image/jyQJdN)
[![classroom.md.png](https://z4a.net/images/2023/12/08/classroom.md.png)](https://z4a.net/image/jyQ1CL)
[![testball.md.png](https://z4a.net/images/2023/12/08/testball.md.png)](https://z4a.net/image/jyQBew)