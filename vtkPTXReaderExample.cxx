#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkXMLPolyDataWriter.h>

#include "vtkPTXReader.h"
#include "vtkPTXData.h"

int main (int argc, char *argv[])
{
  if(argc != 3)
    {
    cout << "Required arguments: InputFilename(ptx) OutputFilename(vtp)" << endl;
    return EXIT_FAILURE;
    }
  
  vtkstd::string inputFilename = argv[1];
  vtkstd::string outputFilename = argv[2];
  
  vtkSmartPointer<vtkPTXReader> reader = 
      vtkSmartPointer<vtkPTXReader>::New();
  reader->SetFileName(inputFilename.c_str());
  reader->Update();
  
  vtkSmartPointer<vtkPolyData> polydata = 
      vtkSmartPointer<vtkPolyData>::New();
  
  //reader->GetOutput()->GeneratePointPolyData(polydata);
  reader->GetOutput()->GenerateTriangulatedPolyData(polydata);
  
  vtkSmartPointer<vtkXMLPolyDataWriter> writer = 
      vtkSmartPointer<vtkXMLPolyDataWriter>::New();
  writer->SetFileName(outputFilename.c_str());
  writer->SetInput(polydata);
  writer->Write();
  
  return EXIT_SUCCESS;
}
