# This is hgtrack/hgtracker/CMakeLists.txt
# Configuration

SET (hgtracker_source
  hgtracker.cxx     hgtracker.h
)  
ADD_LIBRARY (hgtracker ${hgtracker_source})
TARGET_LINK_LIBRARIES (hgtracker  )

#SUBDIRS (example)