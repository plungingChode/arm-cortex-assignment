cmake_minimum_required(VERSION 3.16)

set(CMX_IOC "${CMAKE_CURRENT_LIST_DIR}/gombos.ioc")

# Possible values: openocd, pyocd, stlink. stlink is default
# set(CMX_DEBUGGER "openocd")
# set(OPENOCD_CFG "${CMAKE_CURRENT_SOURCE_DIR}/openocd.cfg")

include(cmake/cubemx.cmake)

project(${CMX_PROJ})
add_executable(${CMX_PROJ})

target_include_directories(${CMX_PROJ} PRIVATE
    "USB_HOST/App"
    "USB_HOST/Target"
    "Middlewares/ST/STM32_USB_Host_Library/Core/Inc"
    "Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Inc"
)
file(GLOB_RECURSE MIDDLEWARE_SRC
    "USB_HOST/*.c"
    "Middlewares/*.c"
)

target_sources(${CMX_PROJ} PRIVATE ${MIDDLEWARE_SRC})
cubemx_target(${CMX_PROJ})
target_compile_options(${CMX_PROJ} PRIVATE -Og -Wall -g -gdwarf-2)

# Depending on the project setup, sometimes one of these symbols must be omitted. (Cannot be reliably derived from the .ioc file)
# target_compile_definitions(${CMX_PROJ} PRIVATE USE_FULL_LL_DRIVER USE_HAL_DRIVER)
target_compile_definitions(${CMX_PROJ} PRIVATE USE_HAL_DRIVER)
