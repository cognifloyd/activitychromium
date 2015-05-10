project (activitychromium)
 
find_package(KDE4 REQUIRED)
include_directories(${KDE4_INCLUDES})
 
set(tutorial2_SRCS 
  main.cpp
)
 
kde4_add_executable(activitychromium ${tutorial2_SRCS})
target_link_libraries(activitychromium ${KDE4_KDEUI_LIBS})
