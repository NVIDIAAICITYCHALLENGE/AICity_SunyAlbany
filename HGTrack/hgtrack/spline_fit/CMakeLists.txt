# This is hgtrack/spline_fit/CMakeLists.txt
# Configuration

SET (spline_fit_source
  alglibinternal.cxx    alglibinternal.h
  alglibmisc.cxx        alglibmisc.h
  ap.cxx                ap.h
  integration.cxx       integration.h
  interpolation.cxx     interpolation.h
  linalg.cxx            linalg.h
  optimization.cxx      optimization.h
  solvers.cxx           solvers.h
  specialfunctions.cxx  specialfunctions.h
)  
ADD_LIBRARY (spline_fit ${spline_fit_source})
TARGET_LINK_LIBRARIES (spline_fit  )

#SUBDIRS (example)