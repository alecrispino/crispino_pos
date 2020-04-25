project('bankomat', 'cpp',
        default_options: ['cpp_std=c++17', 'werror=true']) #'warning_level=3', 

include_dir = include_directories(['include'])
src = (['src/main.cpp', 'src/bankomat.cpp'])
#thread_dep = dependency('threads')

executable('bankomat',
            sources: src, 
            include_directories: include_dir,
#            dependencies: thread_dep
            )