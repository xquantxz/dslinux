/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%         CCCC   OOO   M   M  PPPP    OOO   SSSSS  IIIII  TTTTT  EEEEE        %
%        C      O   O  MM MM  P   P  O   O  SS       I      T    E            %
%        C      O   O  M.M M  PPPP   O   O   SSS     I      T    EEE          %
%        C      O   O  M   M  P      O   O     SS    I      T    E            %
%         CCCC   OOO   M   M  P       OOO   SSSSS  IIIII    T    EEEEE        %
%                                                                             %
%                                                                             %
%                        Digitally composite two images.                      %
%                                                                             %
%                              Software Design                                %
%                                John Cristy                                  %
%                               January 1993                                  %
%                                                                             %
%                                                                             %
%  Copyright 1999-2008 ImageMagick Studio LLC, a non-profit organization      %
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
%  Program Composite composites images to create new images.
%
%
*/

/*
  Include declarations.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "wand/MagickWand.h"
#if defined(__WINDOWS__)
#include <windows.h>
#endif

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%  M a i n                                                                    %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%
*/
int main(int argc,char **argv)
{
  char
    *option;

  ExceptionInfo
    *exception;

  ImageInfo
    *image_info;

  MagickBooleanType
    regard_warnings,
    status;

  register long
    i;

  MagickCoreGenesis(*argv,MagickTrue);
  exception=AcquireExceptionInfo();
  regard_warnings=MagickFalse;
  for (i=1; i < (long) argc; i++)
  {
    option=argv[i];
    if ((strlen(option) == 1) || ((*option != '-') && (*option != '+')))
      continue;
    if (LocaleCompare("debug",option+1) == 0)
      (void) SetLogEventMask(argv[++i]);
    if (LocaleCompare("regard-warnings",option+1) == 0)
      regard_warnings=MagickTrue;
  }
  image_info=AcquireImageInfo();
  status=CompositeImageCommand(image_info,argc,argv,(char **) NULL,exception);
  if (exception->severity != UndefinedException)
    {
      if ((exception->severity > ErrorException) ||
          (regard_warnings != MagickFalse))
        status=MagickTrue;
      CatchException(exception);
    }
  image_info=DestroyImageInfo(image_info);
  exception=DestroyExceptionInfo(exception);
  MagickCoreTerminus();
  return(status == MagickFalse ? 0 : 1);
}
