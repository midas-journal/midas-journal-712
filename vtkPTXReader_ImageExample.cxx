#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkJPEGWriter.h>

#include "vtkPTXReader.h"
#include "vtkPTXData.h"

int main (int argc, char *argv[])
{
  if(argc != 3)
    {
    cout << "Required arguments: InputFilename(ptx) OutputFilename(jpg)" << endl;
    return EXIT_FAILURE;
    }
  
  vtkstd::string inputFilename = argv[1];
  vtkstd::string outputFilename = argv[2];
  
  vtkSmartPointer<vtkPTXReader> reader = 
      vtkSmartPointer<vtkPTXReader>::New();
  reader->SetFileName(inputFilename.c_str());
  reader->Update();
  
  vtkSmartPointer<vtkPTXData> data = reader->GetOutput();
  
  vtkSmartPointer<vtkImageData> image = 
      vtkSmartPointer<vtkImageData>::New();
  data->CreateImage(image);
  
  vtkSmartPointer<vtkJPEGWriter> writer = 
      vtkSmartPointer<vtkJPEGWriter>::New();
  writer->SetFileName(outputFilename.c_str());
  writer->SetInput(image);
  writer->Write();
  
  return EXIT_SUCCESS;
}
