qt5_wrap_ui(forms
  ../ui/mainWindow.ui)
  
set(TTBIN_SRC ttbin/export_csv.c ttbin/export_gpx.c ttbin/export_kml.c ttbin/export_tcx.c ttbin/ttbin.c)
add_library(libttbin STATIC ${TTBIN_SRC})
target_link_libraries(libttbin ${CURL_LIBRARIES})
set_target_properties(libttbin PROPERTIES OUTPUT_NAME ttbin)

add_executable(ttbinEditor main.cpp mainWindow.cpp diagramScene.cpp fileHandler.cpp ${forms})

# Use the Widgets module from Qt 5.
target_link_libraries(ttbinEditor Qt5::Core Qt5::Widgets Qt5::Gui libttbin m ${CURL_LIBRARIES})
