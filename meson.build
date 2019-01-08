project('Skypaper','c')
skypaper_sources = ['skypaper.c', 'util.c', 'window.c', 'render.c', 'shader.c', 'uniforms.c']

include = include_directories('include')
glad_library = shared_library('glad', 'glad/glad.c', dependencies: meson.get_compiler('c').find_library('dl'), include_directories : include, install : false)
glad_dependency = declare_dependency(include_directories : include, link_with : glad_library)

skypaper_dependencies = [dependency('glfw3'), dependency('x11'), dependency('gl'), glad_dependency]
executable('skypaper', sources : skypaper_sources, dependencies : skypaper_dependencies )



build_config = configuration_data()
build_config.set('version', 'Dev 0')
build_config.set('shortlicence', 'Copyright (C) AidoP')
configure_file(input : 'build_config.h.pre', output : 'build_config.h', configuration : build_config)