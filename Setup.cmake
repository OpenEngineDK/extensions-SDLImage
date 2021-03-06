SET(CMAKE_INCLUDE_PATH ${OE_LIB_DIR}/sdlimage/include/SDL)
SET(CMAKE_LIBRARY_PATH ${OE_LIB_DIR}/sdlimage/lib)
INCLUDE(${CMAKE_ROOT}/Modules/FindSDL_image.cmake)
INCLUDE(${OE_CURRENT_EXTENSION_DIR}/SetupSDL_image.cmake)

OE_ADD_SERIALIZABLE_OBJECTS(Extensions_SDLImage
  Resources/SDLImage
)
