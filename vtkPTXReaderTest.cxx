#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkXMLPolyDataWriter.h>

#include "vtkPTXReader.h"
#include "vtkPTXData.h"

int main (int argc, char *argv[])
{
  //verify input arguments
  if(argc != 2)
    {
    cout << "Required arguments: InputFilename(ptx)" << endl;
    return EXIT_FAILURE;
    }
  
  vtkstd::string inputFilename = argv[1];
  
  vtkSmartPointer<vtkPTXReader> reader = 
      vtkSmartPointer<vtkPTXReader>::New();
  reader->SetFileName(inputFilename.c_str());
  reader->Update();
  
  vtkPTXData* polydata = reader->GetOutput();
  return EXIT_SUCCESS;
}
