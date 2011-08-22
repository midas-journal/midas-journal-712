#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkXMLPolyDataReader.h>

#include "vtkPTXReader.h"

int main (int argc, char *argv[])
{
  if(argc != 3)
    {
    cout << "Required arguments: InputFilename(vtp) OutputFilename(vtp)" << endl;
    return EXIT_FAILURE;
    }
  
  vtkstd::string inputFilename = argv[1];
  vtkstd::string outputFilename = argv[2];
  
  vtkSmartPointer<vtkXMLPolyDataReader> reader = 
      vtkSmartPointer<vtkXMLPolyDataReader>::New();
  reader->SetFileName(inputFilename.c_str());
  reader->Update();
  
  vtkSmartPointer<vtkPTXReader> pTXreader = 
      vtkSmartPointer<vtkPTXReader>::New();
  //pTXreader->
      
  vtkSmartPointer<vtkXMLPolyDataWriter> writer = 
      vtkSmartPointer<vtkXMLPolyDataWriter>::New();
  writer->SetFileName(outputFilename.c_str());
  writer->SetInput(reader->GetOutput());
  writer->Write();
  
  return EXIT_SUCCESS;
}
