// .NAME vtkPTXReader - read a ptx file
// .SECTION Description
// vtkPTXReader reads a ptx file

#ifndef __vtkPTXReader_h
#define __vtkPTXReader_h

#include "vtkPTXData.h"

#include "vtkAlgorithm.h"
#include "vtkSmartPointer.h"
#include "vtkDenseArray.h"
#include "vtkVector.h"
#include "vtkColor.h"
#include "vtkImageData.h"

class vtkPTXReader : public vtkAlgorithm  
{
public:
  vtkTypeRevisionMacro(vtkPTXReader,vtkAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  static vtkPTXReader *New();
  
  // Description:
  // Specify file name of the ptx file.
  vtkSetStringMacro(FileName);
  vtkGetStringMacro(FileName);
  
  // Description:
  // Get the output data object.
  vtkPTXData* GetOutput();
    
protected:
  vtkPTXReader();
  ~vtkPTXReader(){}
  
  int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);
  virtual int FillOutputPortInformation( int port, vtkInformation* info );
  virtual int RequestDataObject(
                                  vtkInformation* request,
                                  vtkInformationVector** inputVector,
                                  vtkInformationVector* outputVector );
  int ProcessRequest(vtkInformation* request,
                                     vtkInformationVector** inputVector,
                                     vtkInformationVector* outputVector);
  int RequestUpdateExtent(
                                          vtkInformation* vtkNotUsed(request),
    vtkInformationVector** inputVector,
    vtkInformationVector* vtkNotUsed(outputVector));
  
  int RequestInformation(
                                         vtkInformation* vtkNotUsed(request),
    vtkInformationVector** vtkNotUsed(inputVector),
                                      vtkInformationVector* vtkNotUsed(outputVector));

  private:
  vtkPTXReader(const vtkPTXReader&);  // Not implemented.
  void operator=(const vtkPTXReader&);  // Not implemented.
    
  char* FileName;
  
  //funtions
  void ReadFile(vtkSmartPointer<vtkPTXData> data);
  
};

#endif
