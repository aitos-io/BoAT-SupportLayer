set(SUB_MODULE_NAME T1oI2C_obj)

# add_subdirectory(protos)

file(GLOB_RECURSE FILE_SRC     ${CMAKE_CURRENT_SOURCE_DIR}/*.c)



add_library(${SUB_MODULE_NAME}  OBJECT  ${FILE_SRC})


# add_dependencies(${SUB_MODULE_NAME} protos-obj)
target_include_directories(${SUB_MODULE_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}../vendor
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
