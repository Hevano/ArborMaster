install(
    TARGETS ArborMaster_exe
    RUNTIME COMPONENT ArborMaster_Runtime
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
