set(LIB_NAME BFramework)

file(GLOB PUBLIC_HEADERS CONFIGURE_DEPENDS "Public/*.h")
file(GLOB PRIVATE_SRC     CONFIGURE_DEPENDS "Private/*.cpp")

add_library(${LIB_NAME} SHARED ${PUBLIC_HEADERS} ${PRIVATE_SRC})

target_include_directories(${LIB_NAME}
        PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/Public
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Private
)

# Depends on Core (BObject, Input) and Renderer2D
target_link_libraries(${LIB_NAME}
        PUBLIC BCore BRenderer2D
)

target_compile_definitions(${LIB_NAME}
        PRIVATE BENGINE_FRAMEWORK_BUILD
        PUBLIC  $<$<CONFIG:Debug>:BENGINE_DEBUG>
)

set_target_properties(${LIB_NAME} PROPERTIES OUTPUT_NAME "BFramework")
