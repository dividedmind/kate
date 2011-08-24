set(kateprofilesengine_SRCS
    kateprofilesengine.cpp
    kateprofilesservice.cpp
)

kde4_add_plugin(plasma_engine_kateprofiles ${kateprofilesengine_SRCS})
target_link_libraries(plasma_engine_kateprofiles ${KDE4_PLASMA_LIBS} ${KDE4_KIO_LIBS})

install(TARGETS plasma_engine_kateprofiles DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-dataengine-kateprofiles.desktop DESTINATION ${SERVICES_INSTALL_DIR} )
install(FILES org.kde.plasma.dataengine.kateprofiles.operations DESTINATION ${DATA_INSTALL_DIR}/plasma/services)

