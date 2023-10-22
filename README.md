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

# TODO List
- [ ] Bug: The rasterizer cannot render properly if the scale of the quad is too big.
- [ ] Refactor: The renderTarget implementation is too ugly.

# Gallery
![green-bathroom](https://z4a.net/images/2023/10/22/green-bathroom.md.png)
![classroom](https://z4a.net/images/2023/10/22/classroom.md.png)
![teapot](https://z4a.net/images/2023/10/22/teapot.md.png)
![testball](https://z4a.net/images/2023/10/22/testball.md.png)
