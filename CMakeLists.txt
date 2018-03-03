project( integral_image )

cmake_minimum_required( VERSION 3.6 )

set(isInit on)

if(WIN32)
    set(BOOST_ROOT "C:/boost_1_64_0/boost_mingw_link_static")
else(UNIX)
    set(BOOST_ROOT "/usr/lib/boost_1_66")
endif()

set(BOOST_DIR ${BOOST_ROOT})
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_DEBUG ON)

#Поиск компонетнов boost
find_package(Boost 1.64.0 REQUIRED program_options system COMPONENTS)

set( CMAKE_CXX_STANDARD 14 )
set( CMAKE_INCLUDE_CURRENT_DIR ON )
set( CMAKE_AUTOMOC ON )

include_directories( ${CMAKE_SOURCE_DIR}/src )
include_directories( ${CMAKE_SOURCE_DIR}/include )

set (SOURCE_FILES src/main.cpp
                  src/argparser.cpp
                  src/argparser.h
                  src/integralimage.cpp
                  src/integralimage.h
    )

#Включение директории Boost
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
else()
    set(isInit off)
endif()


find_package( OpenCV REQUIRED )

if(OpenCV_FOUND)
    include_directories( ${OpenCV_INCLUDE_DIRS} )
endif()

#Включение OpenMP
include(FindOpenMP)

if(OPENMP_FOUND)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()


#Сборка компонета ПО
if( isInit )

    add_executable( ${PROJECT_NAME} ${SOURCE_FILES} )

    if(WIN32)
        set(DEPENDENCY_LIB ws2_32 wsock32)
    else(UNIX)
        set(DEPENDENCY_LIB rt pthread )
    endif()

    target_link_libraries( ${PROJECT_NAME} ${Boost_LIBRARIES} ${DEPENDENCY_LIB} ${OpenCV_LIBS})

else()
    message(STATUS "*** ERROR *** Build is down")
endif()
