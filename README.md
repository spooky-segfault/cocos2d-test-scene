# Cocos2d-x test scene

A simple Cocos2d-x scene. Project uses C++ programming language and represents a scrollable parallax game scene with simple animations and user interface.

## Useful links

- [Cocos2d-x repo](https://github.com/cocos2d/cocos2d-x)
- [Software Installation](https://docs.cocos2d-x.org/cocos2d-x/v4/en/installation/)
- [Command-Line tool](https://docs.cocos2d-x.org/cocos2d-x/v4/en/editors_and_tools/cocosCLTool.html)

## Build

### Desktop

```bash
cmake -S . -B build
cmake --build build
```

### Android

Open `proj.android` folder in Android Studio.

## Notes

List of packages necessary to build cocos2d-x on Fedora
```bash
sudo dnf install python2.7 gcc-c++ gdk-pixbuf2-devel cmake mesa-libGLU-devel \
                 gl2ps-devel libXi-devel libzip-devel libcurl-devel \
                 fontconfig-devel libsqlite3x-devel glew-devel openssl-devel \
                 gtk3-devel glfw-devel @"X Software Development"
```
