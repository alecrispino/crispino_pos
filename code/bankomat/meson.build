project('bankomat', 'cpp',
        default_options: ['warning_level=3', 'cpp_std=c++17', 'werror=true'])

include_dir = include_directories(['include'])
src = (['src/main.cpp'])
#thread_dep = dependency('threads')

executable('bankomat',
            sources: src, 
            include_directories: include_dir,
#            dependencies: thread_dep
            )