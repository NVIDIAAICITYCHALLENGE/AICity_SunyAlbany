# This is hgtrack/detector/CMakeLists.txt
# Configuration

SET (detector_source
  detector.cxx     detector.h
)  
ADD_LIBRARY (detector ${detector_source})
TARGET_LINK_LIBRARIES (detector  )

#SUBDIRS (example)