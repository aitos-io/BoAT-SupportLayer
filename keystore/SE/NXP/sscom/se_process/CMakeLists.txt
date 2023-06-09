set(SUB_MODULE_NAME         se_process_obj)

file(GLOB_RECURSE FILE_SRC     ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

add_library(${SUB_MODULE_NAME}  OBJECT  ${FILE_SRC})


# target_link_libraries(${SUB_MODULE_NAME}  boatvendor cjson-obj  protocol_obj protos-obj)

target_include_directories(${SUB_MODULE_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/sscom/include
    ${PROJECT_SOURCE_DIR}/sscom/smCom
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)


