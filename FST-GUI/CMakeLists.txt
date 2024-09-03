# Add new source files here

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

link_directories(${GTK3_LIBRARY_DIRS})
link_directories("/opt/intel/oneapi/2024.2/lib/")

add_executable(FST-GUI
  "BlockQueue.cpp"
  "FST-GUI.cpp"
  "FST-GUI.hpp"
  "GPUFrame.cpp"
  "GPUFrame.hpp"
  "Logging.hpp"
  "Logging.cpp"
  "MainFrame.cpp"
  "MainFrame.hpp"
  "RunThread.cpp"
  "RunThread.hpp"
  "subprocess.h"
  "utils.cpp"
  "utils.hpp"
)

set(CMAKE_C_COMPILER "/opt/intel/oneapi/2024.2/bin/icx")
set(CMAKE_CXX_COMPILER "/opt/intel/oneapi/2024.2/bin/icpx")

target_include_directories(FST-GUI PRIVATE ${GTK3_INCLUDE_DIRS})
target_include_directories(FST-GUI PRIVATE /usr/local/include/wx-3.2/)
target_include_directories(FST-GUI PRIVATE /usr/local/lib/wx/include/gtk3-unicode-static-3.2/)
target_include_directories(FST-GUI PRIVATE ..)

target_link_libraries(FST-GUI PRIVATE ${GTK3_LIBRARIES})
target_link_libraries(FST-GUI PRIVATE /usr/local/lib/libwx_baseu-3.2.a)
target_link_libraries(FST-GUI PRIVATE /usr/local/lib/libwx_gtk3u_core-3.2.a)
target_link_libraries(FST-GUI PRIVATE ${GTK3_LIBRARIES})
target_link_libraries(FST-GUI PRIVATE "-lX11 -lpangoft2-1.0 -lfontconfig -lfontconfig -lxkbcommon -lpng16 -lSM")
target_link_libraries(FST-GUI PRIVATE "-lcudart")
target_link_libraries(FST-GUI PRIVATE "-lsycl")

target_link_libraries(FST-GUI PRIVATE 
        CUDAQuery
        SYCLQuery
)

set_target_properties(FST-GUI PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED yes
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_FILE_OFFSET_BITS=64 -D__WXGTK__ -pthread ${GTK3_CFLAGS_OTHER}")

if (SUPPORTS_IPO)
  message(STATUS "Link-time optimization supported")
  set_target_properties(main PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION yes
  )
else()
  message(STATUS "Link-time optimization not supported")
endif()
