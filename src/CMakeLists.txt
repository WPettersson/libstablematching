SET(SOURCES
  Agent.cpp
  smti.cpp
  smti_grp.cpp
  smti_preprocessing.cpp
  smti_ip.cpp
  Graph.cpp
  )

ADD_LIBRARY(smti SHARED ${SOURCES})
TARGET_LINK_LIBRARIES(smti ${LIBRARIES})

ADD_EXECUTABLE(main main.cpp)
TARGET_LINK_LIBRARIES(main smti)
ADD_EXECUTABLE(convert_wpmaxsat convert_wpmaxsat.cpp)
TARGET_LINK_LIBRARIES(convert_wpmaxsat smti)
ADD_EXECUTABLE(pbo pbo.cpp)
TARGET_LINK_LIBRARIES(pbo smti)
ADD_EXECUTABLE(pbo_convert pbo_convert.cpp)
TARGET_LINK_LIBRARIES(pbo_convert smti)
ADD_EXECUTABLE(pp pp.cpp)
TARGET_LINK_LIBRARIES(pp smti)

