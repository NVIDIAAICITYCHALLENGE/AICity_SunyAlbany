# This is hgtrack/export/CMakeLists.txt
# Configuration

SET (export_source
  export.cxx      export.h
)  
ADD_LIBRARY (export ${export_source})
TARGET_LINK_LIBRARIES (export  )

#SUBDIRS (example)