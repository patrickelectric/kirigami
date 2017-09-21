

set(libkirigami_SRCS
    platformtheme.cpp
    basictheme.cpp
)

ecm_qt_declare_logging_category(libkirigami_SRCS
    HEADER loggingcategory.h
    IDENTIFIER KirigamiLog
    CATEGORY_NAME kf5.kirigami
    DEFAULT_SEVERITY Warning
)

add_library(KF5Kirigami ${libkirigami_SRCS})
generate_export_header(KF5Kirigami BASE_NAME Kirigami)
add_library(KF5::Kirigami ALIAS KF5Kirigami)

target_include_directories(KF5Kirigami
    INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF5}/Kirigami>"
)

target_link_libraries(KF5Kirigami
    PUBLIC
        Qt5::Core
    PRIVATE
        Qt5::Qml
        Qt5::Quick
)

set_target_properties(KF5Kirigami PROPERTIES
    VERSION     ${KIRIGAMI_VERSION_STRING}
    SOVERSION   ${KIRIGAMI_SOVERSION}
    EXPORT_NAME "Kirigami"
)

ecm_generate_headers(Kirigami_HEADERS
    HEADER_NAMES
    PlatformTheme

    REQUIRED_HEADERS Kirigami_HEADERS
)

install(TARGETS KF5Kirigami
        EXPORT KF5KirigamiTargets
        ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})


install(FILES
            ${CMAKE_CURRENT_BINARY_DIR}/kirigami_export.h
            ${Kirigami_HEADERS}
        DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/Kirigami
        COMPONENT Devel)

if(BUILD_QCH)
    ecm_add_qch(
        KF5Kirigami_QCH
        NAME Kirigami
        BASE_NAME KF5Kirigami
        VERSION ${KF5_VERSION}
        ORG_DOMAIN org.kde
        SOURCES # using only public headers, to cover only public API
            ${Kirigami_HEADERS}
        MD_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md"
        LINK_QCHS
            Qt5Core_QCH
        BLANK_MACROS
            KIRIGAMI_EXPORT
            KIRIGAMI_DEPRECATED
        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        COMPONENT Devel
    )
endif()

include(ECMGeneratePriFile)
ecm_generate_pri_file(BASE_NAME Kirigami LIB_NAME KF5Kirigami DEPS "core" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR_KF5}/Kirigami)
install(FILES ${PRI_FILENAME}
        DESTINATION ${ECM_MKSPECS_INSTALL_DIR})

