set(SUB_MODULE_NAME         keypair_obj)

set(FILE_SRC    boatkeypair.c)

add_library(${SUB_MODULE_NAME}  OBJECT  ${FILE_SRC})


# target_link_libraries(${SUB_MODULE_NAME}  boatvendor cjson-obj  protocol_obj protos-obj)

target_include_directories(${SUB_MODULE_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/sdk/include
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/BoAT-SupportLayer/platform/include
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)


