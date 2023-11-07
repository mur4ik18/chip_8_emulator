project('chip_8', 'c',
  version : '0.1',
  default_options : ['warning_level=3'])

sdl2_dep = dependency('sdl2')
internal_inc = include_directories('.')




executable('chip_8',
           'main.c',
	   dependencies: [sdl2_dep],
           install : true)
