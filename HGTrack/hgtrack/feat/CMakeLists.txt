# This is hgtrack/feat/CMakeLists.txt
# Configuration

SET (feat_source
  feat_class.cxx     feat_class.h
  feat_funs.cxx      feat_funs.h
)  
ADD_LIBRARY (feat ${feat_source})
TARGET_LINK_LIBRARIES (feat  )

#SUBDIRS (example)