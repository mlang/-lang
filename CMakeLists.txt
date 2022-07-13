cmake_minimum_required(VERSION 3.18)
project(mlang)
set(CMAKE_BUILD_TYPE MinSizeRel)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
find_program(Reflex_EXECUTABLE reflex HINTS ~/.local/bin)
find_library(Reflex_LIBRARY reflexmin HINTS ~/.local/lib REQUIRED)
find_path(Reflex_INCLUDE_DIR reflex/matcher.h HINTS ~/.local/include)
add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/scanner.cxx
         ${CMAKE_CURRENT_BINARY_DIR}/scanner.hxx
  COMMAND ${Reflex_EXECUTABLE}
  ARGS -o ${CMAKE_CURRENT_BINARY_DIR}/scanner.cxx
      --header-file=${CMAKE_CURRENT_BINARY_DIR}/scanner.hxx
      ${PROJECT_SOURCE_DIR}/lang.lxx
  DEPENDS ${PROJECT_SOURCE_DIR}/lang.lxx
)
find_program(BISON_EXECUTABLE bison)
add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/parser.cxx
         ${CMAKE_CURRENT_BINARY_DIR}/parser.hxx
         ${CMAKE_CURRENT_BINARY_DIR}/location.hxx
  COMMAND ${BISON_EXECUTABLE}
  ARGS --output=${CMAKE_CURRENT_BINARY_DIR}/parser.cxx
       --defines=${CMAKE_CURRENT_BINARY_DIR}/parser.hxx
       --define='api.location.file="${CMAKE_CURRENT_BINARY_DIR}/location.hxx"'
       ${PROJECT_SOURCE_DIR}/lang.yxx
  DEPENDS ${PROJECT_SOURCE_DIR}/lang.yxx
)
 
add_executable(lang lang.cxx ${CMAKE_CURRENT_BINARY_DIR}/scanner.cxx ${CMAKE_CURRENT_BINARY_DIR}/parser.cxx)
target_include_directories(lang
  PRIVATE ${Reflex_INCLUDE_DIR}
          ${CMAKE_CURRENT_BINARY_DIR}
) 
target_link_libraries(lang ${Reflex_LIBRARY})
