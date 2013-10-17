GET_FILENAME_COMPONENT(myDir ${CMAKE_CURRENT_LIST_FILE} PATH)

SET(QJSON_LIBRARIES qjson)
SET(QJSON_INCLUDE_DIR "/home/barry963/Botnia/download_file/qjson-0.8.1/build/_preferred_path_/include")

include(${myDir}/QJSONTargets.cmake)
