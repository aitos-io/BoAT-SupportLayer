set(SUB_MODULE_NAME smCom_obj)
# set(MODULE_NAME smCom)

# add_subdirectory(protos)

file(GLOB_RECURSE FILE_SRC     ${CMAKE_CURRENT_SOURCE_DIR}/*.c)



add_library(${SUB_MODULE_NAME}  OBJECT  ${FILE_SRC})

add_subdirectory(T1oI2C)

# add_library(${MODULE_NAME}  STATIC  $<TARGET_OBJECTS:smCom-obj>
#                                     $<TARGET_OBJECTS:T1oI2C-obj>)

# add_dependencies(${SUB_MODULE_NAME} protos-obj)
target_include_directories(${SUB_MODULE_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/T1oI2C
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
