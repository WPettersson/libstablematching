SET(SOURCES
  Agent.cpp
  smti.cpp
  smti_grp.cpp
  smti_preprocessing.cpp
  smti_ip.cpp
  smti_encodings.cpp
  Graph.cpp
  )

ADD_LIBRARY(smti SHARED ${SOURCES})
TARGET_LINK_LIBRARIES(smti ${LIBRARIES} coverage_config)

