**Skypaper** - Live glsl wallpaper for Linux

## Installation
**Dependencies**

The [Meson](https://mesonbuild.com/index.html) build system
- meson
- pkg-config

Libraries
- libx11
- glfw3
- gl

Currently Linux is a hard dependency.

**Compiling**

Compilation is easy with [Meson](https://mesonbuild.com/index.html).
```bash
$ git clone https://gitlab.com/AidoP1/skypaper.git
$ cd skypaper
$ meson build
$ cd build
$ ninja
```

You can now copy the binary to a safe place.
You might want to put it out of the way and create a .desktop file to run it
```bash
$ chmod +x skypaper
$ mkdir $XDG_HOME_DATA/skypaper
$ cp skypaper $XDG_HOME_DATA/skypaper
```

## Usage
Skypaper can be started directly after compilation to show the boring default shader.
For a better wallpaper, place a fragment shader at `~/.local/share/skypaper/shaders/<shader_name>/frag.glsl` and execute skypaper with the `-s <shader_name>` argument. 

## Writing Shaders
Skypaper defaults to OpenGL 3.3.0. Specify this by including the following at the top of your shaders.
```glsl
#version 330 core
```

#### TODO

#### Fragment Shader
Inputs
- `in vec2 fragCoord`

Outputs
- `out vec4 fragColor`

**Default Shader**

```glsl
#version 330 core
out vec4 fragColor;
in vec2 fragCoord;
void main() {
    fragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
```

## License
Skypaper is provided under the GNU General Public License 3.
GLAD and the contents of `include/` and `glad/` are not covered by this license
unless otherwise stated within the file.

```
    Copyright (C) 2019  Aidan Prangnell (AidoP)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or 
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    A full copy of the license should be included with the source. If not see https://www.gnu.org/licenses/.
```