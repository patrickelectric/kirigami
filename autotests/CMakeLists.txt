add_test(NAME qmltests COMMAND qmltestrunner WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

set_property(TEST qmltests PROPERTY ENVIRONMENT 
"QML2_IMPORT_PATH=${CMAKE_BINARY_DIR}/bin")
