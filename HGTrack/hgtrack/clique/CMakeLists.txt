# This is hgtrack/clique/CMakeLists.txt
# Configuration

SET (clique_source
  cliquer.cxx    cliquer.h
  cliquerconf.h  
  graph.cxx      graph.h
  misc.cxx       misc.h
  reorder.cxx    reorder.h
  set.h
)  
ADD_LIBRARY (clique ${clique_source})
TARGET_LINK_LIBRARIES (clique  )

#SUBDIRS (example)