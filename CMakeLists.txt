cmake_minimum_required(VERSION 2.6)

PROJECT(vtkPTXReader)
ENABLE_TESTING()

FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})

SET(BUILD_PARAVIEW_PLUGIN OFF CACHE BOOL "Build Paraview plugin?")

if(BUILD_PARAVIEW_PLUGIN)
  FIND_PACKAGE(ParaView REQUIRED)
  INCLUDE(${PARAVIEW_USE_FILE})
  
  ADD_PARAVIEW_PLUGIN(PTXReader "1.0"
  SERVER_MANAGER_XML vtkPTXReader.xml
  SERVER_MANAGER_SOURCES vtkPTXReaderPlugin.cxx
  SERVER_SOURCES vtkPTXReader.cxx vtkPTXData.cxx
  GUI_RESOURCE_FILES vtkPTXReader_GUI.xml
  )
endif(BUILD_PARAVIEW_PLUGIN)

###############
ADD_EXECUTABLE(vtkPTXReaderExample vtkPTXReaderExample.cxx
vtkPTXReader.cxx vtkPTXData.cxx)
TARGET_LINK_LIBRARIES(vtkPTXReaderExample vtkHybrid )

ADD_EXECUTABLE(vtkPTXReader_ImageExample vtkPTXReader_ImageExample.cxx
vtkPTXReader.cxx vtkPTXData.cxx)
TARGET_LINK_LIBRARIES(vtkPTXReader_ImageExample vtkHybrid )

ADD_EXECUTABLE(vtkPTXReaderPluginExample vtkPTXReaderPluginExample.cxx
vtkPTXReaderPlugin.cxx vtkPTXReader.cxx vtkPTXData.cxx)
TARGET_LINK_LIBRARIES(vtkPTXReaderPluginExample vtkHybrid )

###############
ADD_EXECUTABLE(vtkPTXReaderTest vtkPTXReaderTest.cxx vtkPTXReader.cxx vtkPTXData.cxx)
TARGET_LINK_LIBRARIES(vtkPTXReaderTest vtkHybrid )
ADD_TEST(vtkPTXReaderTest vtkPTXReaderTest input.ptx)
