/*
 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 %                                                                             %
 %                                                                             %
 %                                                                             %
 %                     IIIIIIIIII    PPPPPPPP      LL                          %
 %                         II        PP      PP    LL                          %
 %                         II        PP       PP   LL                          %
 %                         II        PP      PP    LL                          %
 %                         II        PPPPPPPP      LL                          %
 %                         II        PP            LL                          %
 %                         II        PP            LL                          %
 %                     IIIIIIIIII    PP            LLLLLLLL                    %
 %                                                                             %
 %                                                                             %
 %                                                                             %
 %                   Read/Write Scanalytics IPLab Image Format                 %
 %                                  Sean Burke                                 %
 %                                  2008.05.07                                 %
 %                                     v 0.9                                   %
 %                                                                             %
 %  Copyright 1999-2007 ImageMagick Studio LLC, a non-profit organization      %
 %  dedicated to making software imaging solutions freely available.           %
 %                                                                             %
 %  You may not use this file except in compliance with the License.  You may  %
 %  obtain a copy of the License at                                            %
 %                                                                             %
 %    http://www.imagemagick.org/script/license.php                            %
 %                                                                             %
 %  Unless required by applicable law or agreed to in writing, software        %
 %  distributed under the License is distributed on an "AS IS" BASIS,          %
 %  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   %
 %  See the License for the specific language governing permissions and        %
 %  limitations under the License.                                             %
 %                                                                             %
 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 %
 %
 */

/*
 Include declarations.
 */
#include "magick/studio.h"
#include "magick/blob.h"
#include "magick/blob-private.h"
#include "magick/colorspace.h"
#include "magick/exception.h"
#include "magick/exception-private.h"
#include "magick/image.h"
#include "magick/image-private.h"
#include "magick/list.h"
#include "magick/magick.h"
#include "magick/memory_.h"
#include "magick/monitor.h"
#include "magick/option.h"
#include "magick/property.h"
#include "magick/quantum-private.h"
#include "magick/static.h"
#include "magick/string_.h"
#include "magick/module.h"

/* 
Tyedef declarations
 */

typedef struct _IPLInfo
{
  unsigned long
  tag,
  size,
  time,
  z,
  width,
  height,
  colors,
  depth,
  byteType;
} IPLInfo;

void increase (void *pixel, int byteType){
  switch(byteType){
    case 0:(*((unsigned char *) pixel))++; break;
    case 1:(*((signed int *) pixel))++; break;
    case 2:(*((unsigned int *) pixel))++; break;
    case 3:(*((signed long *) pixel))++; break;
    default:(*((unsigned int *) pixel))++; break;
  }  
}

/*
 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 %                                                                             %
 %                                                                             %
 %                                                                             %
 %   I s I P L                                                                 %
 %                                                                             %
 %                                                                             %
 %                                                                             %
 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 %
 %  IsIPL() returns MagickTrue if the image format type, identified by the
 %  magick string, is IPL.
 %
 %  The format of the IsIPL method is:
 %
 %      MagickBooleanType IsIPL(const unsigned char *magick,const size_t length)
 %
 %  A description of each parameter follows:
 %
 %    o magick: This string is generally the first few bytes of an image file
 %      or blob.
 %
 %    o length: Specifies the length of the magick string.
 %
 */
static MagickBooleanType IsIPL(const unsigned char *magick,const size_t length)
{
  if (length < 4)
    return(MagickFalse);
  if (LocaleNCompare((char *) magick,"data",4) == 0)
    return(MagickTrue);
  return(MagickFalse);
}

/*
 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 %                                                                             %
 %                                                                             %
 %                                                                             %
 %    R e a d I P L I m a g e                                                  %
 %                                                                             %
 %                                                                             %
 %                                                                             %
 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 %
 %  ReadIPLImage() reads a Scanalytics IPLab image file and returns it.  It 
 %  allocates the memory necessary for the new Image structure and returns a 
 %  pointer to the new image.
 %
 %  According to the IPLab spec, the data is blocked out in five dimensions:
 %  { t, z, c, y, x }.  When we return the image, the latter three are folded
 %  into the standard "Image" structure.  The "scenes" (image_info->scene) 
 %  correspond to the order: { {t0,z0}, {t0, z1}, ..., {t1,z0}, {t1,z1}... }
 %  The number of scenes is t*z.
 %
 %  The format of the ReadIPLImage method is:
 %
 %      Image *ReadIPLImage(const ImageInfo *image_info,ExceptionInfo *exception)
 %
 %  A description of each parameter follows:
 %
 %    o image_info: The image info.
 %
 %    o exception: return any errors or warnings in this structure. 
 %
 */

void SetHeaderFromIPL(Image *image, IPLInfo *ipl){
	image->columns = ipl->width;
	image->rows = ipl->height;
	image->depth = ipl->depth;
	image->x_resolution = 1;
	image->y_resolution = 1;
}


static Image *ReadIPLImage(const ImageInfo *image_info,ExceptionInfo *exception)
{
  
  /* 
  Declare variables 
   */
  Image *image;
  MagickBooleanType status;
  register PixelPacket *q;
  unsigned char magick[12], *pixels;
  ssize_t count;
  long y;
  unsigned long t_count=0;
  size_t length;
  IPLInfo
    ipl_info;
  QuantumFormatType
    quantum_format;
  QuantumInfo
    *quantum_info;
  QuantumType
    quantum_type;

  /*
   Open Image
   */

  assert(image_info != (const ImageInfo *) NULL);
  assert(image_info->signature == MagickSignature);
  if ( image_info->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent, GetMagickModule(), "%s",
                image_info->filename);
  assert(exception != (ExceptionInfo *) NULL);
  assert(exception->signature == MagickSignature);
  image=AcquireImage(image_info);
  status=OpenBlob(image_info,image,ReadBinaryBlobMode,exception);
  if (status == MagickFalse)
  {
    image=DestroyImageList(image);
    return((Image *) NULL);
  }
  
  /*
   Read IPL image
   */

  /* 
    Determine endianness 
   If we get back "iiii", we have LSB,"mmmm", MSB
   */
  count=ReadBlob(image,4,magick); 
  if((LocaleNCompare((char *) magick,"iiii",4) == 0))  
    image->endian=LSBEndian;
  else{
    if((LocaleNCompare((char *) magick,"mmmm",4) == 0)) 
      image->endian=MSBEndian;
    else{
      ThrowReaderException(CorruptImageError, "ImproperImageHeader");
    }
  }
  /* Skip o'er the next 8 bytes (garbage) */
  count=ReadBlob(image, 8, magick); 
  /*
   Excellent, now we read the header unimpeded.
   */
  count=ReadBlob(image,4,magick); 
  if((LocaleNCompare((char *) magick,"data",4) != 0))  
    ThrowReaderException(CorruptImageError, "ImproperImageHeader");
  ipl_info.size=ReadBlobLong(image); 
  ipl_info.width=ReadBlobLong(image); 
  ipl_info.height=ReadBlobLong(image); 
  if((ipl_info.width == 0UL) || (ipl_info.height == 0UL))
    ThrowReaderException(CorruptImageError,"ImproperImageHeader");
  ipl_info.colors=ReadBlobLong(image); 
  if(ipl_info.colors == 3){ image->colorspace=RGBColorspace;}
  else { image->colorspace = GRAYColorspace; }
  ipl_info.z=ReadBlobLong(image); 
  ipl_info.time=ReadBlobLong(image); 

  ipl_info.byteType=ReadBlobLong(image); 


  /* Initialize Quantum Info */

  switch (ipl_info.byteType) {
    case 0: 
      ipl_info.depth=8;
      quantum_format = UnsignedQuantumFormat;
      break;
    case 1: 
      ipl_info.depth=16;
      quantum_format = SignedQuantumFormat;
      break;
    case 2: 
      ipl_info.depth=16;
      quantum_format = UnsignedQuantumFormat;
      break;
    case 3: 
      ipl_info.depth=32;
      quantum_format = SignedQuantumFormat;
      break;
    case 4: ipl_info.depth=32;
      quantum_format = FloatingPointQuantumFormat;
      break;
    case 5: 
      ipl_info.depth=8;
      quantum_format = UnsignedQuantumFormat;
      break;
    case 6: 
      ipl_info.depth=16;
      quantum_format = UnsignedQuantumFormat;
      break;
    case 10:  
      ipl_info.depth=64;
      quantum_format = FloatingPointQuantumFormat;
      break; 
    default: 
      ipl_info.depth=16;
      quantum_format = UnsignedQuantumFormat;
      break;
  }

  /*
    Set number of scenes of image
  */

  SetHeaderFromIPL(image, &ipl_info);

  /* Thats all we need if we are pinging. */
  if (image_info->ping != MagickFalse)
  {
    (void) CloseBlob(image);
    return(GetFirstImageInList(image));
  }
  length=image->columns;
  quantum_type=GetQuantumType(image,exception);
 do
  {
    SetHeaderFromIPL(image, &ipl_info);

  if ((image_info->ping != MagickFalse) && (image_info->number_scenes != 0))
      if (image->scene >= (image_info->scene+image_info->number_scenes-1))
        break;
  if (SetImageExtent(image,0,0) == MagickFalse)
    {
      InheritException(exception,&image->exception);
      return(DestroyImageList(image));
    }
/*
   printf("Length: %lu, Memory size: %lu\n", length,(unsigned long)(image->depth));
*/
     quantum_info=AcquireQuantumInfo(image_info,image);
     (void) SetQuantumFormat(image,quantum_format,quantum_info);
     pixels=GetQuantumPixels(quantum_info); 
     if(image->columns != ipl_info.width){
/*
 		printf("Columns not set correctly!  Wanted: %lu, got: %lu\n",
 			ipl_info.width, image->columns);
*/
     }

    /* 
    Covert IPL binary to pixel packets
     */
    
  if(ipl_info.colors == 1){
      for(y = 0; y < (long) image->rows; y++){
        (void) ReadBlob(image, length*image->depth/8, pixels);
        q=SetImagePixels(image,0,y,image->columns,1);
        if (q == (PixelPacket *) NULL)
                break;
	(void) ImportQuantumPixels(image,quantum_info,GrayQuantum,pixels);
          if (SyncImagePixels(image) == MagickFalse)
                  break;
  	}
  }
  else{
      for(y = 0; y < (long) image->rows; y++){
        (void) ReadBlob(image, length*image->depth/8, pixels);
        q=SetImagePixels(image,0,y,image->columns,1);
        if (q == (PixelPacket *) NULL)
                break;
	(void) ImportQuantumPixels(image,quantum_info,RedQuantum,pixels);	
        if (SyncImagePixels(image) == MagickFalse)
          break;
      }
      for(y = 0; y < (long) image->rows; y++){
        (void) ReadBlob(image, length*image->depth/8, pixels);
        q=SetImagePixels(image,0,y,image->columns,1);
        if (q == (PixelPacket *) NULL)
                break;
	(void) ImportQuantumPixels(image,quantum_info,GreenQuantum,pixels);
        if (SyncImagePixels(image) == MagickFalse)
          break;
      }
      for(y = 0; y < (long) image->rows; y++){
        (void) ReadBlob(image, length*image->depth/8, pixels);
        q=SetImagePixels(image,0,y,image->columns,1);
        if (q == (PixelPacket *) NULL)
                break;
	(void) ImportQuantumPixels(image,quantum_info,BlueQuantum,pixels);
        if (SyncImagePixels(image) == MagickFalse)
          break;
      }
   }
   SetQuantumImageType(image,quantum_type);
 
    t_count++;
  quantum_info = DestroyQuantumInfo(quantum_info);

    if (EOFBlob(image) != MagickFalse)
    {
      ThrowFileException(exception,CorruptImageError,"UnexpectedEndOfFile",
                 image->filename);
      break;
    }
   if(t_count < ipl_info.z * ipl_info.time){
      /*
       Proceed to next image.
       */
      AcquireNextImage(image_info, image);
      if (GetNextImageInList(image) == (Image *) NULL)
      {
        image=DestroyImageList(image);
        return((Image *) NULL);
      }
      image=SyncNextImageInList(image); 

      if (image->progress_monitor != (MagickProgressMonitor) NULL)
      {
        status=image->progress_monitor(LoadImagesTag,TellBlob(image),
        GetBlobSize(image),image->client_data);
        if (status == MagickFalse)
          break;
      }
    }
  } while (t_count < ipl_info.z*ipl_info.time);
  CloseBlob(image);
  return(GetFirstImageInList(image));
}

/*
 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 %                                                                             %
 %                                                                             %
 %                                                                             %
 %   W r i t e I P L I m a g e                                                 %
 %                                                                             %
 %                                                                             %
 %                                                                             %
 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 %
 %  WriteIPLImage() writes an image to a file in Scanalytics IPLabimage format.
 %
 %  The format of the WriteIPLImage method is:
 %
 %      MagickBooleanType WriteIPLImage(const ImageInfo *image_info,Image *image)
 %
 %  A description of each parameter follows.
 %
 %    o image_info: The image info.
 %
 %    o image:  The image.
 %
 */

static MagickBooleanType WriteIPLImage(const ImageInfo *image_info,Image *image)
{
  MagickBooleanType
    status;
  
  MagickOffsetType
    scene;
  
  register const PixelPacket
    *p;

  unsigned char
  *pixels;
 
  long
    y;
  
  IPLInfo
    ipl_info;

  QuantumInfo
    *quantum_info;

   /*
    Open output image file.
  */
  assert(image_info != (const ImageInfo *) NULL);
  assert(image_info->signature == MagickSignature);
  assert(image != (Image *) NULL);
  assert(image->signature == MagickSignature);
  if (image->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",image->filename);
  status=OpenBlob(image_info,image,WriteBinaryBlobMode,&image->exception);
  if (status == MagickFalse)
    return(status);
  scene=0;
  

  quantum_info=AcquireQuantumInfo(image_info, image);
  if ((quantum_info->format == UndefinedQuantumFormat) &&
      (IsHighDynamicRangeImage(image,&image->exception) != MagickFalse))
    (void) SetQuantumFormat(image,FloatingPointQuantumFormat,quantum_info);
  switch(quantum_info->depth){
  case 8: 
    ipl_info.byteType = 0;
    break;
  case 16:
    if(quantum_info->format == SignedQuantumFormat){
      ipl_info.byteType = 2;
    }
    else{
      ipl_info.byteType = 1;
    }
    break;
  case 32:
    if(quantum_info->format == FloatingPointQuantumFormat){
      ipl_info.byteType = 3;
    }
    else{
      ipl_info.byteType = 4;
    }
    break;
  case 64:
    ipl_info.byteType = 10;
    break;
  default: 
    ipl_info.byteType = 2; 
    break;
    
  }
  ipl_info.z = GetImageListLength(image);
  /* There is no current method for detecting whether we have T or Z stacks */
  ipl_info.time = 1;
  ipl_info.width = image->columns;
  ipl_info.height = image->rows;
  
  if (image->colorspace != RGBColorspace)
    (void) SetImageColorspace(image,RGBColorspace);
  
  if(image->colorspace == RGBColorspace) { ipl_info.colors = 3; }
  else{ ipl_info.colors = 1; }
  
  ipl_info.size = 28 + 
    ((image->depth)/8)*ipl_info.height*ipl_info.width*ipl_info.colors*ipl_info.z;
  
  /* Ok!  Calculations are done.  Lets write this puppy down! */
  
  /*
    Write IPL header.
  */
  /* Shockingly (maybe not if you have used IPLab),  IPLab itself CANNOT read MSBEndian
  files!   The reader above can, but they cannot.  For compatability reasons, I will leave
  the code in here, but it is all but useless if you want to use IPLab. */

  if(image_info->endian == MSBEndian)
    (void) WriteBlob(image, 4, (unsigned char *) "mmmm");
  else{
    image->endian = LSBEndian;
    (void) WriteBlob(image, 4, (unsigned char *) "iiii");
  }
  (void) WriteBlobLong(image, 4);
  (void) WriteBlob(image, 4, (unsigned char *) "100f");
  (void) WriteBlob(image, 4, (unsigned char *) "data");
  (void) WriteBlobLong(image, ipl_info.size);
  (void) WriteBlobLong(image, ipl_info.width); 
  (void) WriteBlobLong(image, ipl_info.height);
  (void) WriteBlobLong(image, ipl_info.colors);
  if(image_info->adjoin == MagickFalse)
  (void) WriteBlobLong(image, 1);
  else
  (void) WriteBlobLong(image, ipl_info.z);
  (void) WriteBlobLong(image, ipl_info.time);
  (void) WriteBlobLong(image, ipl_info.byteType);
  

  
 do
    {
      /*
  Convert MIFF to IPL raster pixels.
      */
      pixels=GetQuantumPixels(quantum_info);
  if(ipl_info.colors == 1){
  /* Red frame */
  for(y = 0; y < (long) ipl_info.height; y++){
    p=GetImagePixels(image,0,y,image->columns,1);
    if (p == (PixelPacket *) NULL)
      break;
      (void) ExportQuantumPixels(image, quantum_info, GrayQuantum, pixels);
      (void) WriteBlob(image, image->columns*image->depth/8, pixels);
  }

}
  if(ipl_info.colors == 3){
  /* Red frame */
  for(y = 0; y < (long) ipl_info.height; y++){
    p=GetImagePixels(image,0,y,image->columns,1);
    if (p == (PixelPacket *) NULL)
      break;
      (void) ExportQuantumPixels(image, quantum_info, RedQuantum, pixels);
      (void) WriteBlob(image, image->columns*image->depth/8, pixels);
  }

    /* Green frame */
    for(y = 0; y < (long) ipl_info.height; y++){
      p=AcquireImagePixels(image,0,y,image->columns,1,&image->exception);
      if (p == (PixelPacket *) NULL)
        break;
        (void) ExportQuantumPixels(image, quantum_info, GreenQuantum, pixels);
        (void) WriteBlob(image, image->columns*image->depth/8, pixels);
    }
    /* Blue frame */
    for(y = 0; y < (long) ipl_info.height; y++){
      p=AcquireImagePixels(image,0,y,image->columns,1,&image->exception);
      if (p == (PixelPacket *) NULL)
        break;
      (void) ExportQuantumPixels(image, quantum_info, BlueQuantum, pixels);
      (void) WriteBlob(image, image->columns*image->depth/8, pixels);
      if (image->previous == (Image *) NULL){
        if ((image->progress_monitor != (MagickProgressMonitor) NULL) &&
      (QuantumTick((MagickOffsetType) y,image->rows) != MagickFalse))
    {
      status=image->progress_monitor(SaveImageTag,(MagickOffsetType) y,image->rows, image->client_data);
      if (status == MagickFalse) break;
    }
      }
    }
  }
  quantum_info=DestroyQuantumInfo(quantum_info);
      if (GetNextImageInList(image) == (Image *) NULL)
  break;
      image=SyncNextImageInList(image);
      if (image->progress_monitor != (MagickProgressMonitor) NULL)
  {
    status=image->progress_monitor(SaveImagesTag,scene, GetImageListLength(image),image->client_data);
    if (status == MagickFalse)
      break;
  }
      scene++;
    }while (image_info->adjoin != MagickFalse);

  (void) WriteBlob(image, 4, (unsigned char *) "fini");
  (void) WriteBlobLong(image, 0);

CloseBlob(image);
return(MagickTrue);
}


/*
 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 %                                                                             %
 %                                                                             %
 %                                                                             %
 %   R e g i s t e r I P L I m a g e                                           %
 %                                                                             %
 %                                                                             %
 %                                                                             %
 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 %
 % RegisterIPLImage() add attributes for the Scanalytics IPL image format to the
 % list of supported formats.  
 %
 %
 */
ModuleExport unsigned long RegisterIPLImage(void)
{
  MagickInfo
    *entry;
  
  entry=SetMagickInfo("IPL");
  entry->decoder=(DecodeImageHandler *) ReadIPLImage;
  entry->encoder=(EncodeImageHandler *) WriteIPLImage;
  entry->magick=(IsImageFormatHandler *) IsIPL;
  entry->adjoin=MagickTrue;
  entry->description=ConstantString("IPL Image Sequence");
  entry->module=ConstantString("IPL");
  entry->endian_support=MagickTrue;
  (void) RegisterMagickInfo(entry);
  return(MagickImageCoderSignature);
}

/*
 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 %                                                                             %
 %                                                                             %
 %                                                                             %
 %   U n r e g i s t e r I P L I m a g e                                       %
 %                                                                             %
 %                                                                             %
 %                                                                             %
 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 %
 %  UnregisterIPLImage() removes format registrations made by the
 %  IPL module from the list of supported formats.
 %
 %  The format of the UnregisterIPLImage method is:
 %
 %      UnregisterIPLImage(void)
 %
 */
ModuleExport void UnregisterIPLImage(void)
{
  (void) UnregisterMagickInfo("IPL");
}
