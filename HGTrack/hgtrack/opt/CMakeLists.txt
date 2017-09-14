# This is hgtrack/opt/CMakeLists.txt
# Configuration

SET (opt_source
  video_segment_graph.cxx     video_segment_graph.h
  hierarch_hypergraph.cxx     hierarch_hypergraph.h
)  
ADD_LIBRARY (opt ${opt_source})
TARGET_LINK_LIBRARIES (opt  )

#SUBDIRS (example)