# This is hgtrack/config/CMakeLists.txt
# Configuration

SET (config_source
  config.cxx     config.h
  param.cxx      param.h
)  
ADD_LIBRARY (config ${config_source})
TARGET_LINK_LIBRARIES (config  )

#SUBDIRS (example)