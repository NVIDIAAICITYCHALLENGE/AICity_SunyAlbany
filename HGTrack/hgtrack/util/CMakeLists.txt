# This is hgtrack/util/CMakeLists.txt
# Configuration

SET (util_source
  tframe.cxx    tframe.h
  tracklet.cxx  tracklet.h 
  defs.cxx      defs.h
)  
ADD_LIBRARY (util ${util_source})
TARGET_LINK_LIBRARIES (util  )

#SUBDIRS (example)