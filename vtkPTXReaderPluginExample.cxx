#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkXMLPolyDataWriter.h>

#include "vtkPTXReaderPlugin.h"

int main (int argc, char *argv[])
{
  if(argc != 3)
    {
    cout << "Required arguments: InputFilename(ptx) OutputFilename(vtp)" << endl;
    return EXIT_FAILURE;
    }
  
  vtkstd::string inputFilename = argv[1];
  vtkstd::string outputFilename = argv[2];
  
  vtkSmartPointer<vtkPTXReaderPlugin> reader = 
      vtkSmartPointer<vtkPTXReaderPlugin>::New();
  reader->SetFileName(inputFilename.c_str());
  reader->Update();
  
  vtkSmartPointer<vtkPolyData> polydata = 
      reader->GetOutput();
  
  vtkSmartPointer<vtkXMLPolyDataWriter> writer = 
      vtkSmartPointer<vtkXMLPolyDataWriter>::New();
  writer->SetFileName(outputFilename.c_str());
  writer->SetInput(polydata);
  writer->Write();
  
  return EXIT_SUCCESS;
}
