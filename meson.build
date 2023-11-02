project('chip_8', 'c',
  version : '0.1',
  default_options : ['warning_level=3'])

sdl2_dep = dependency('sdl2')
internal_inc = include_directories('.')
#mylib = library('emulator', 'emulator.h')
emulatorlib = shared_library('emulator', sources : ['emulator.h','emulator.c'])
graphiclib = shared_library('graphic', sources : ['graphic.h', 'graphic.c'], dependencies: sdl2_dep) 

executable('chip_8',
           'main.c',
	   dependencies: [sdl2_dep],
	   link_with: [emulatorlib, graphiclib],
           install : true)
