# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/FCheddar_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/FCheddar_autogen.dir/ParseCache.txt"
  "FCheddar_autogen"
  )
endif()
