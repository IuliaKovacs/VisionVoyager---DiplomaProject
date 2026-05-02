# HardwareAbstractionLayer/CMakeLists.txt

# Define the library name
# We use INTERFACE because it contains mainly header files (.h)
add_library(HardwareAbstractionLayer INTERFACE)

# Export the directory so other modules can do #include "HardwareInterfaces.h"
# PUBLIC ensures that anyone linking to this library gets the include path automatically
target_include_directories(HardwareAbstractionLayer INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

