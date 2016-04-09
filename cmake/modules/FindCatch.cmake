# - Try to find Catch
# Once done this will define
#  FOUND_Catch - If Catch headers were found
#  Catch_INCLUDE_DIRS - The Catch include directory

find_path(Catch_INCLUDE_DIR catch.hpp)

if (Catch_INCLUDE_DIR)
  set(FOUND_Catch TRUE)
  message(STATUS "Found header for Catch")
endif (Catch_INCLUDE_DIR)

mark_as_advanced(Catch_INCLUDE_DIRS)

