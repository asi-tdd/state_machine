cmake_minimum_required(VERSION 3.15)
project(state_machine LANGUAGES CXX)

set(STATE_MACHINE_CLASSES
	StateMachine
	CallableState
)

foreach(CLASS ${STATE_MACHINE_CLASSES})
    list(APPEND STATE_MACHINE_SOURCES src/${CLASS}.cpp)
	list(APPEND STATE_MACHINE_INCLUDES include/state_machine/${CLASS}.h)
endforeach()

# create and export lib
add_library(${PROJECT_NAME} STATIC ${STATE_MACHINE_SOURCES} ${STATE_MACHINE_INCLUDES})
target_include_directories(${PROJECT_NAME} PUBLIC include)

# Add tests, one per class
foreach(CLASS ${STATE_MACHINE_CLASSES})
	add_gtest(${CLASS}_tests test/tests/${CLASS}_tests.cpp "test" ${PROJECT_NAME} ${PROJECT_NAME}_tests)
endforeach()
