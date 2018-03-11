project( integral_image )

cmake_minimum_required( VERSION 3.6 )

set(isInit on)

#Подключение библиотеки Boost
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

#Использование стандарта с++ 14
set( CMAKE_CXX_STANDARD 14 )
set( CMAKE_INCLUDE_CURRENT_DIR ON )
set( CMAKE_AUTOMOC ON )

include_directories( ${CMAKE_SOURCE_DIR}/src )

# Список файлов исходного кода
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

#Поиск библиотеки OpenCV
find_package( OpenCV )

if(OpenCV_FOUND)
    include_directories( ${OpenCV_INCLUDE_DIRS} )
endif()

#Включение стандарта OpenMP
include(FindOpenMP)

if(OPENMP_FOUND)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()


#Сборка компонета ПО
if( isInit )

    add_executable( ${PROJECT_NAME} ${SOURCE_FILES} )

    target_link_libraries( ${PROJECT_NAME} ${Boost_LIBRARIES} ${OpenCV_LIBS})

else()
    message(STATUS "*** ERROR *** Build is down")
endif()


#Генерация документации Doxygen
if(BUILD_DOXYGEN_DOCS)

    #Поиск пакета генерации документации Doxygen
    find_package(Doxygen)

    if(DOXYGEN_FOUND)

        set(doxyfile_in ${CMAKE_SOURCE_DIR}/doxyfile/Doxyfile)

        add_custom_target(
          doc ALL
          COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
          COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile_in}
          WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doxyfile
          COMMENT "Generating API documentation with Doxygen"
          VERBATIM
        )
    else()
        message(STATUS "doxygen not found")
    endif()
endif()

#Сборка юнит тестов с использованием google test frameworkЖ
if(BUILD_GTEST)

    set(GTEST_ROOT ${CMAKE_SOURCE_DIR}/external/googletest/googletest)

    add_subdirectory(${GTEST_ROOT})

    include_directories(${gtest_SOURCE_DIR}/include ${COMMON_INCLUDES})

    #include_directories(${GTEST_INCLUDE_DIRS})

    set(test_name "integral_image_test")

    #Файлы юнит тестов
    set(src_test_files gtests/main.cc
                       gtests/argparser.cc
                       gtests/integralimage.cc
        )


    add_library( ${PROJECT_NAME}_lib ${SOURCE_FILES} )
    target_link_libraries( ${PROJECT_NAME}_lib ${Boost_LIBRARIES} ${OpenCV_LIBS})
    add_executable( ${test_name} ${src_test_files} )

    target_link_libraries( ${test_name} gtest ${PROJECT_NAME}_lib)

endif()
