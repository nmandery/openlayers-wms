# Find Qt4
ADD_DEFINITIONS( -DQT_STATICPLUGIN )
FIND_PACKAGE( Qt4 REQUIRED )
FIND_PACKAGE( FastCgiQt REQUIRED )

# need gui for webkit
SET( QT_DONT_USE_QTGUI FALSE )
SET( QT_USE_WEBKIT TRUE )
SET( QT_USE_QTNETWORK TRUE )

# Include the cmake file needed to use qt4
INCLUDE( ${QT_USE_FILE} )

# build
SET( MOC_SOURCES )
SET( HEADERS )
SET( SOURCES )
FILE( GLOB HEADERS *.h )
FILE( GLOB SOURCES *.cpp )
QT4_WRAP_CPP( MOC_SOURCES ${HEADERS} )
ADD_EXECUTABLE( openlayers-wms.fcgi ${SOURCES} ${MOC_SOURCES} )

# webkit is not linked when only QT_LIBRARIES are specified,
# explicit adding of QT_QTWEBKIT_LIBRARY is necesary
TARGET_LINK_LIBRARIES(openlayers-wms.fcgi 
        ${QT_LIBRARIES} 
        ${QT_QTWEBKIT_LIBRARY} 
        ${FASTCGIQT_LIBRARIES})

SET_TARGET_PROPERTIES( openlayers-wms.fcgi PROPERTIES RUNTIME_OUTPUT_DIRECTORY . )

#install
install( TARGETS openlayers-wms.fcgi RUNTIME DESTINATION lib/cgi-bin )

