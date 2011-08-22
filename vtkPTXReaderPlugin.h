// .NAME vtkPTXReader - read a ptx file
// .SECTION Description
// vtkPTXReader reads a ptx file

#ifndef __vtkPTXReaderPlugin_h
#define __vtkPTXReaderPlugin_h

#include "vtkPolyDataAlgorithm.h"
#include "vtkSmartPointer.h"

class vtkPTXReaderPlugin : public vtkPolyDataAlgorithm  
{
public:
  vtkTypeRevisionMacro(vtkPTXReaderPlugin,vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  static vtkPTXReaderPlugin *New();
  
  // Description:
  // Specify file name of the ptx file.
  vtkSetStringMacro(FileName);
  vtkGetStringMacro(FileName);
    
  vtkSetMacro(Triangulate, bool);
  vtkGetMacro(Triangulate, bool);
  
  vtkSetMacro(SpanGaps, bool);
  vtkGetMacro(SpanGaps, bool);
  
protected:
  vtkPTXReaderPlugin();
  ~vtkPTXReaderPlugin(){}
  
  int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);
  
private:
  vtkPTXReaderPlugin(const vtkPTXReaderPlugin&);  // Not implemented.
  void operator=(const vtkPTXReaderPlugin&);  // Not implemented.
    
  char* FileName;
  
  bool Triangulate;
  bool SpanGaps;
};

#endif
