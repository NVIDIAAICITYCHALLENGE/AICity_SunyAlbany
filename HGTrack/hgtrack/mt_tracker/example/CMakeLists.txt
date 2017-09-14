# This is hgtrack/mt_tracker/example/CMakeLists.txt
LINK_DIRECTORIES( ${LIBRARY_OUTPUT_PATH} )
ADD_EXECUTABLE (mt_tracker_example mt_tracker_example.cxx)

# Different order may cause undefined reference error
# https://stackoverflow.com/questions/16574113/cmake-linking-error-undefined-reference-to
TARGET_LINK_LIBRARIES (mt_tracker_example 
    hgtracker opt export util
    config detector ${OpenCV_LIBS}
    feat graph_decomp spline_fit
    math clique
)

