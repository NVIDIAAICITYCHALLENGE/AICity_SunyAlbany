# This is hgtrack/graph_decomp/CMakeLists.txt
# Graph Decompose

SET (graph_decomp_source
  detect_clusters.cxx    detect_clusters.h
  graph_decomp.cxx       graph_decomp.h
)  
ADD_LIBRARY (graph_decomp ${graph_decomp_source})
TARGET_LINK_LIBRARIES (graph_decomp ${OpenCV_LIBS})

SUBDIRS (example)