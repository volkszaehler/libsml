file(GLOB SOURCES "sml/src/*.c")
idf_component_register(SRCS ${SOURCES} INCLUDE_DIRS "sml/include")