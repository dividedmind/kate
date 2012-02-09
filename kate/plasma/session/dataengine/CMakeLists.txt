set(katesessionsengine_SRCS
    kateprofilesengine.cpp
    kateprofilesservice.cpp
)

kde4_add_plugin(plasma_engine_katesessions ${katesessionsengine_SRCS})
target_link_libraries(plasma_engine_katesessions ${KDE4_PLASMA_LIBS} ${KDE4_KIO_LIBS})

install(TARGETS plasma_engine_katesessions DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-dataengine-katesessions.desktop DESTINATION ${SERVICES_INSTALL_DIR} )
install(FILES org.kde.katesessions.operations DESTINATION ${DATA_INSTALL_DIR}/plasma/services)

