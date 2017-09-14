# This is hgtrack/math/CMakeLists.txt
# Configuration

SET (math_source
  algebra.cxx     algebra.h
  matrix.cxx      matrix.h
  vector.cxx      vector.h
)  
ADD_LIBRARY (math ${math_source})
TARGET_LINK_LIBRARIES (math  )

#SUBDIRS (example)