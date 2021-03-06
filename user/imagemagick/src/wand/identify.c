/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%           IIIII  DDDD   EEEEE  N   N  TTTTT  IIIII  FFFFF  Y   Y            %
%             I    D   D  E      NN  N    T      I    F       Y Y             %
%             I    D   D  EEE    N N N    T      I    FFF      Y              %
%             I    D   D  E      N  NN    T      I    F        Y              %
%           IIIII  DDDD   EEEEE  N   N    T    IIIII  F        Y              %
%                                                                             %
%                                                                             %
%               Identify an Image Format and Characteristics.                 %
%                                                                             %
%                           Software Design                                   %
%                             John Cristy                                     %
%                            September 1994                                   %
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
%  Identify describes the format and characteristics of one or more image
%  files.  It will also report if an image is incomplete or corrupt.
%
%
*/

/*
  Include declarations.
*/
#include "wand/studio.h"
#include "wand/MagickWand.h"
#include "wand/mogrify-private.h"

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
+   I d e n t i f y I m a g e C o m m a n d                                   %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  IdentifyImageCommand() describes the format and characteristics of one or
%  more image files. It will also report if an image is incomplete or corrupt.
%  The information displayed includes the scene number, the file name, the
%  width and height of the image, whether the image is colormapped or not,
%  the number of colors in the image, the number of bytes in the image, the
%  format of the image (JPEG, PNM, etc.), and finally the number of seconds
%  it took to read and process the image.
%
%  The format of the IdentifyImageCommand method is:
%
%      MagickBooleanType IdentifyImageCommand(ImageInfo *image_info,int argc,
%        char **argv,char **metadata,ExceptionInfo *exception)
%
%  A description of each parameter follows:
%
%    o image_info: the image info.
%
%    o argc: the number of elements in the argument vector.
%
%    o argv: A text array containing the command line arguments.
%
%    o metadata: any metadata is returned here.
%
%    o exception: Return any errors or warnings in this structure.
%
*/

static void IdentifyUsage(void)
{
  const char
    **p;

  static const char
    *miscellaneous[]=
    {
      "-debug events        display copious debugging information",
      "-help                print program options",
      "-list type           print a list of supported option arguments",
      "-log format          format of debugging information",
      "-version             print version information",
      (char *) NULL
    },
    *settings[]=
    {
      "-alpha option        activate, deactivate, reset, or set the alpha channel",
      "-antialias           remove pixel-aliasing",
      "-authenticate password",
      "                     decipher image with this password",
      "-channel type        apply option to select image channels",
      "-colorspace type     alternate image colorspace",
      "-crop geometry       cut out a rectangular region of the image",
      "-define format:option",
      "                     define one or more image format options",
      "-density geometry    horizontal and vertical density of the image",
      "-depth value         image depth",
      "-extract geometry    extract area from image",
      "-format \"string\"     output formatted image characteristics",
      "-fuzz distance       colors within this distance are considered equal",
      "-gamma value         level of gamma correction",
      "-interlace type      type of image interlacing scheme",
      "-interpolate method  pixel color interpolation method",
      "-limit type value    pixel cache resource limit",
      "-monitor             monitor progress",
      "-ping                efficiently determine image attributes",
      "-quiet               suppress all warning messages",
      "-regard-warnings     pay attention to warning messages",
      "-respect-parenthesis settings remain in effect until parenthesis boundary",
      "-sampling-factor geometry",
      "                     horizontal and vertical sampling factor",
      "-seed value          seed a new sequence of pseudo-random numbers",
      "-set attribute value set an image attribute",
      "-size geometry       width and height of image",
      "-strip               strip image of all profiles and comments",
      "-units type          the units of image resolution",
      "-verbose             print detailed information about the image",
      "-virtual-pixel method",
      "                     virtual pixel access method",
      (char *) NULL
    };

  (void) printf("Version: %s\n",GetMagickVersion((unsigned long *) NULL));
  (void) printf("Copyright: %s\n\n",GetMagickCopyright());
  (void) printf("Usage: %s [options ...] file [ [options ...] "
    "file ... ]\n",GetClientName());
  (void) printf("\nImage Settings:\n");
  for (p=settings; *p != (char *) NULL; p++)
    (void) printf("  %s\n",*p);
  (void) printf("\nMiscellaneous Options:\n");
  for (p=miscellaneous; *p != (char *) NULL; p++)
    (void) printf("  %s\n",*p);
  (void) printf(
    "\nBy default, the image format of `file' is determined by its magic\n");
  (void) printf(
    "number.  To specify a particular image format, precede the filename\n");
  (void) printf(
    "with an image format name and a colon (i.e. ps:image) or specify the\n");
  (void) printf(
    "image type as the filename suffix (i.e. image.ps).  Specify 'file' as\n");
  (void) printf("'-' for standard input or output.\n");
  exit(0);
}

WandExport MagickBooleanType IdentifyImageCommand(ImageInfo *image_info,
  int argc,char **argv,char **metadata,ExceptionInfo *exception)
{
#define DestroyIdentify() \
{ \
  DestroyImageStack(); \
  for (i=0; i < (long) argc; i++) \
    argv[i]=DestroyString(argv[i]); \
  argv=(char **) RelinquishMagickMemory(argv); \
}
#define ThrowIdentifyException(asperity,tag,option) \
{ \
  (void) ThrowMagickException(exception,GetMagickModule(),asperity,tag,"`%s'", \
    option); \
  DestroyIdentify(); \
  return(MagickFalse); \
}
#define ThrowIdentifyInvalidArgumentException(option,argument) \
{ \
  (void) ThrowMagickException(exception,GetMagickModule(),OptionError, \
    "InvalidArgument","`%s': %s",argument,option); \
  DestroyIdentify(); \
  return(MagickFalse); \
}

  const char
    *format,
    *option;

  Image
    *image;

  ImageStack
    image_stack[MaxImageStackDepth+1];

  long
    j,
    k;

  MagickBooleanType
    fire,
    pend;

  MagickStatusType
    status;

  register long
    i;

  unsigned long
    count;

  /*
    Set defaults.
  */
  assert(image_info != (ImageInfo *) NULL);
  assert(image_info->signature == MagickSignature);
  if (image_info->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"...");
  assert(exception != (ExceptionInfo *) NULL);
  if (argc == 2)
    {
      option=argv[1];
      if ((LocaleCompare("version",option+1) == 0) ||
          (LocaleCompare("-version",option+1) == 0))
        {
          (void) fprintf(stdout,"Version: %s\n",
            GetMagickVersion((unsigned long *) NULL));
          (void) fprintf(stdout,"Copyright: %s\n\n",GetMagickCopyright());
          return(MagickTrue);
        }
    }
  if (argc < 2)
    IdentifyUsage();
  count=0;
  format=NULL;
  j=1;
  k=0;
  NewImageStack();
  option=(char *) NULL;
  pend=MagickFalse;
  status=MagickTrue;
  /*
    Identify an image.
  */
  ReadCommandlLine(argc,&argv);
  status=ExpandFilenames(&argc,&argv);
  if (status == MagickFalse)
    ThrowIdentifyException(ResourceLimitError,"MemoryAllocationFailed",
      strerror(errno));
  for (i=1; i < (long) argc; i++)
  {
    option=argv[i];
    if (LocaleCompare(option,"(") == 0)
      {
        FireImageStack(MagickFalse,MagickTrue,pend);
        if (k == MaxImageStackDepth)
          ThrowIdentifyException(OptionError,"ParenthesisNestedTooDeeply",
            option);
        PushImageStack();
        continue;
      }
    if (LocaleCompare(option,")") == 0)
      {
        FireImageStack(MagickFalse,MagickTrue,MagickTrue);
        if (k == 0)
          ThrowIdentifyException(OptionError,"UnableToParseExpression",option);
        PopImageStack();
        continue;
      }
    if (IsMagickOption(option) == MagickFalse)
      {
        char
          *filename;

        Image
          *images;

        ImageInfo
          *identify_info;

        /*
          Read input image.
        */
        FireImageStack(MagickFalse,MagickFalse,pend);
        identify_info=CloneImageInfo(image_info);
        identify_info->verbose=MagickFalse;
        filename=argv[i];
        if ((LocaleCompare(filename,"--") == 0) && (i < (argc-1)))
          filename=argv[++i];
        (void) CopyMagickString(identify_info->filename,filename,MaxTextExtent);
        if (identify_info->ping == MagickFalse)
          images=ReadImage(identify_info,exception);
        else
          images=PingImage(identify_info,exception);
        identify_info=DestroyImageInfo(identify_info);
        status&=(images != (Image *) NULL) &&
          (exception->severity < ErrorException);
        if (images == (Image *) NULL)
          continue;
        AppendImageStack(images);
        FireImageStack(MagickFalse,MagickFalse,MagickTrue);
        for ( ; image != (Image *) NULL; image=GetNextImageInList(image))
        {
          if (image->scene == 0)
            image->scene=count++;
          if (format == (char *) NULL)
            {
              (void) IdentifyImage(image,stdout,image_info->verbose);
              continue;
            }
          if (metadata != (char **) NULL)
            {
              char
                *text;

              text=InterpretImageProperties(image_info,image,format);
              if (text == (char *) NULL)
                ThrowIdentifyException(ResourceLimitError,
                  "MemoryAllocationFailed",strerror(errno));
              (void) ConcatenateString(&(*metadata),text);
              text=DestroyString(text);
              if (LocaleCompare(format,"%n") == 0)
                break;
            }
        }
        RemoveAllImageStack();
        continue;
      }
    pend=image != (Image *) NULL ? MagickTrue : MagickFalse;
    switch (*(option+1))
    {
      case 'a':
      {
        if (LocaleCompare("alpha",option+1) == 0)
          {
            long
              type;

            if (*option == '+')
              break;
            i++;
            if (i == (long) argc)
              ThrowIdentifyException(OptionError,"MissingArgument",option);
            type=ParseMagickOption(MagickAlphaOptions,MagickFalse,argv[i]);
            if (type < 0)
              ThrowIdentifyException(OptionError,"UnrecognizedAlphaChannelType",
                argv[i]);
            break;
          }
        if (LocaleCompare("antialias",option+1) == 0)
          break;
        if (LocaleCompare("authenticate",option+1) == 0)
          {
            if (*option == '+')
              break;
            i++;
            if (i == (long) (argc-1))
              ThrowIdentifyException(OptionError,"MissingArgument",option);
            break;
          }
        ThrowIdentifyException(OptionError,"UnrecognizedOption",option)
      }
      case 'c':
      {
        if (LocaleCompare("cache",option+1) == 0)
          {
            if (*option == '+')
              break;
            i++;
            if (i == (long) argc)
              ThrowIdentifyException(OptionError,"MissingArgument",option);
            if (IsGeometry(argv[i]) == MagickFalse)
              ThrowIdentifyInvalidArgumentException(option,argv[i]);
            break;
          }
        if (LocaleCompare("channel",option+1) == 0)
          {
            long
              channel;

            if (*option == '+')
              break;
            i++;
            if (i == (long) (argc-1))
              ThrowIdentifyException(OptionError,"MissingArgument",option);
            channel=ParseChannelOption(argv[i]);
            if (channel < 0)
              ThrowIdentifyException(OptionError,"UnrecognizedChannelType",
                argv[i]);
            break;
          }
        if (LocaleCompare("colorspace",option+1) == 0)
          {
            long
              colorspace;

            if (*option == '+')
              break;
            i++;
            if (i == (long) (argc-1))
              ThrowIdentifyException(OptionError,"MissingArgument",option);
            colorspace=ParseMagickOption(MagickColorspaceOptions,
              MagickFalse,argv[i]);
            if (colorspace < 0)
              ThrowIdentifyException(OptionError,"UnrecognizedColorspace",
                argv[i]);
            break;
          }
        if (LocaleCompare("crop",option+1) == 0)
          {
            if (*option == '+')
              break;
            i++;
            if (i == (long) (argc-1))
              ThrowIdentifyException(OptionError,"MissingArgument",option);
            if (IsGeometry(argv[i]) == MagickFalse)
              ThrowIdentifyInvalidArgumentException(option,argv[i]);
            image_info->ping=MagickFalse;
            break;
          }
        ThrowIdentifyException(OptionError,"UnrecognizedOption",option)
      }
      case 'd':
      {
        if (LocaleCompare("debug",option+1) == 0)
          {
            long
              event;

            if (*option == '+')
              break;
            i++;
            if (i == (long) argc)
              ThrowIdentifyException(OptionError,"MissingArgument",option);
            event=ParseMagickOption(MagickLogEventOptions,MagickFalse,argv[i]);
            if (event < 0)
              ThrowIdentifyException(OptionError,"UnrecognizedEventType",
                argv[i]);
            (void) SetLogEventMask(argv[i]);
            break;
          }
        if (LocaleCompare("define",option+1) == 0)
          {
            i++;
            if (i == (long) argc)
              ThrowIdentifyException(OptionError,"MissingArgument",option);
            if (*option == '+')
              {
                const char
                  *define;

                define=GetImageOption(image_info,argv[i]);
                if (define == (const char *) NULL)
                  ThrowIdentifyException(OptionError,"NoSuchOption",argv[i]);
                break;
              }
            break;
          }
        if (LocaleCompare("density",option+1) == 0)
          {
            if (*option == '+')
              break;
            i++;
            if (i == (long) argc)
              ThrowIdentifyException(OptionError,"MissingArgument",option);
            if (IsGeometry(argv[i]) == MagickFalse)
              ThrowIdentifyInvalidArgumentException(option,argv[i]);
            break;
          }
        if (LocaleCompare("depth",option+1) == 0)
          {
            if (*option == '+')
              break;
            i++;
            if (i == (long) argc)
              ThrowIdentifyException(OptionError,"MissingArgument",option);
            if (IsGeometry(argv[i]) == MagickFalse)
              ThrowIdentifyInvalidArgumentException(option,argv[i]);
            break;
          }
        ThrowIdentifyException(OptionError,"UnrecognizedOption",option)
      }
      case 'f':
      {
        if (LocaleCompare("format",option+1) == 0)
          {
            format=(char *) NULL;
            if (*option == '+')
              break;
            i++;
            if (i == (long) argc)
              ThrowIdentifyException(OptionError,"MissingArgument",option);
            format=argv[i];
            break;
          }
        if (LocaleCompare("fuzz",option+1) == 0)
          {
            if (*option == '+')
              break;
            i++;
            if (i == (long) (argc-1))
              ThrowIdentifyException(OptionError,"MissingArgument",option);
            if (IsGeometry(argv[i]) == MagickFalse)
              ThrowIdentifyInvalidArgumentException(option,argv[i]);
            break;
          }
        ThrowIdentifyException(OptionError,"UnrecognizedOption",option)
      }
      case 'g':
      {
        if (LocaleCompare("gamma",option+1) == 0)
          {
            i++;
            if (i == (long) (argc-1))
              ThrowIdentifyException(OptionError,"MissingArgument",option);
            if (IsGeometry(argv[i]) == MagickFalse)
              ThrowIdentifyInvalidArgumentException(option,argv[i]);
            break;
          }
        ThrowIdentifyException(OptionError,"UnrecognizedOption",option)
      }
      case 'h':
      {
        if ((LocaleCompare("help",option+1) == 0) ||
            (LocaleCompare("-help",option+1) == 0))
          IdentifyUsage();
        ThrowIdentifyException(OptionError,"UnrecognizedOption",option)
      }
      case 'i':
      {
        if (LocaleCompare("interlace",option+1) == 0)
          {
            long
              interlace;

            if (*option == '+')
              break;
            i++;
            if (i == (long) argc)
              ThrowIdentifyException(OptionError,"MissingArgument",option);
            interlace=ParseMagickOption(MagickInterlaceOptions,MagickFalse,
              argv[i]);
            if (interlace < 0)
              ThrowIdentifyException(OptionError,
                "UnrecognizedInterlaceType",argv[i]);
            break;
          }
        if (LocaleCompare("interpolate",option+1) == 0)
          {
            long
              interpolate;

            if (*option == '+')
              break;
            i++;
            if (i == (long) argc)
              ThrowIdentifyException(OptionError,"MissingArgument",option);
            interpolate=ParseMagickOption(MagickInterpolateOptions,MagickFalse,
              argv[i]);
            if (interpolate < 0)
              ThrowIdentifyException(OptionError,
                "UnrecognizedInterpolateMethod",argv[i]);
            break;
          }
        ThrowIdentifyException(OptionError,"UnrecognizedOption",option)
      }
      case 'l':
      {
        if (LocaleCompare("limit",option+1) == 0)
          {
            char
              *p;

            long
              resource;

            if (*option == '+')
              break;
            i++;
            if (i == (long) argc)
              ThrowIdentifyException(OptionError,"MissingArgument",option);
            resource=ParseMagickOption(MagickResourceOptions,MagickFalse,
              argv[i]);
            if (resource < 0)
              ThrowIdentifyException(OptionError,"UnrecognizedResourceType",
                argv[i]);
            i++;
            if (i == (long) argc)
              ThrowIdentifyException(OptionError,"MissingArgument",option);
            (void) strtod(argv[i],&p);
            if ((p == argv[i]) && (LocaleCompare("unlimited",argv[i]) != 0))
              ThrowIdentifyInvalidArgumentException(option,argv[i]);
            break;
          }
        if (LocaleCompare("list",option+1) == 0)
          {
            long
              list;

            if (*option == '+')
              break;
            i++;
            if (i == (long) argc)
              ThrowIdentifyException(OptionError,"MissingArgument",option);
            list=ParseMagickOption(MagickListOptions,MagickFalse,argv[i]);
            if (list < 0)
              ThrowIdentifyException(OptionError,"UnrecognizedListType",
                argv[i]);
            (void) MogrifyImageInfo(image_info,(int) (i-j+1),(const char **)
              argv+j,exception);
            DestroyIdentify();
            return(MagickTrue);
          }
        if (LocaleCompare("log",option+1) == 0)
          {
            if (*option == '+')
              break;
            i++;
            if ((i == (long) argc) ||
                (strchr(argv[i],'%') == (char *) NULL))
              ThrowIdentifyException(OptionError,"MissingArgument",option);
            break;
          }
        ThrowIdentifyException(OptionError,"UnrecognizedOption",option)
      }
      case 'm':
      {
        if (LocaleCompare("matte",option+1) == 0)
          break;
        if (LocaleCompare("monitor",option+1) == 0)
          break;
        ThrowIdentifyException(OptionError,"UnrecognizedOption",option)
      }
      case 'p':
      {
        if (LocaleCompare("ping",option+1) == 0)
          break;
        ThrowIdentifyException(OptionError,"UnrecognizedOption",option)
      }
      case 'q':
      {
        if (LocaleCompare("quiet",option+1) == 0)
          break;
        ThrowIdentifyException(OptionError,"UnrecognizedOption",option)
      }
      case 'r':
      {
        if (LocaleCompare("regard-warnings",option+1) == 0)
          break;
        if (LocaleCompare("respect-parenthesis",option+1) == 0)
          {
            respect_parenthesis=(*option == '-') ? MagickTrue : MagickFalse;
            break;
          }
        ThrowIdentifyException(OptionError,"UnrecognizedOption",option)
      }
      case 's':
      {
        if (LocaleCompare("sampling-factor",option+1) == 0)
          {
            if (*option == '+')
              break;
            i++;
            if (i == (long) argc)
              ThrowIdentifyException(OptionError,"MissingArgument",option);
            if (IsGeometry(argv[i]) == MagickFalse)
              ThrowIdentifyInvalidArgumentException(option,argv[i]);
            break;
          }
        if (LocaleCompare("seed",option+1) == 0)
          {
            unsigned long
              seed;

            if (*option == '+')
              {
                seed=(unsigned long) time((time_t *) NULL);
                SeedPseudoRandomGenerator(seed);
                break;
              }
            i++;
            if (i == (long) (argc-1))
              ThrowIdentifyException(OptionError,"MissingArgument",option);
            if (IsGeometry(argv[i]) == MagickFalse)
              ThrowIdentifyInvalidArgumentException(option,argv[i]);
            seed=(unsigned long) atol(argv[i]);
            SeedPseudoRandomGenerator(seed);
            break;
          }
        if (LocaleCompare("set",option+1) == 0)
          {
            i++;
            if (i == (long) argc)
              ThrowIdentifyException(OptionError,"MissingArgument",option);
            if (*option == '+')
              break;
            i++;
            if (i == (long) argc)
              ThrowIdentifyException(OptionError,"MissingArgument",option);
            break;
          }
        if (LocaleCompare("size",option+1) == 0)
          {
            if (*option == '+')
              break;
            i++;
            if (i == (long) argc)
              ThrowIdentifyException(OptionError,"MissingArgument",option);
            if (IsGeometry(argv[i]) == MagickFalse)
              ThrowIdentifyInvalidArgumentException(option,argv[i]);
            break;
          }
        if (LocaleCompare("strip",option+1) == 0)
          break;
        if (LocaleCompare("support",option+1) == 0)
          {
            if (*option == '+')
              break;
            i++;
            if (i == (long) argc)
              ThrowIdentifyException(OptionError,"MissingArgument",option);
            if (IsGeometry(argv[i]) == MagickFalse)
              ThrowIdentifyInvalidArgumentException(option,argv[i]);
            break;
          }
        ThrowIdentifyException(OptionError,"UnrecognizedOption",option)
      }
      case 'u':
      {
        if (LocaleCompare("units",option+1) == 0)
          {
            long
              units;

            if (*option == '+')
              break;
            i++;
            if (i == (long) (argc-1))
              ThrowIdentifyException(OptionError,"MissingArgument",option);
            units=ParseMagickOption(MagickResolutionOptions,MagickFalse,
              argv[i]);
            if (units < 0)
              ThrowIdentifyException(OptionError,"UnrecognizedUnitsType",
                argv[i]);
            break;
          }
        ThrowIdentifyException(OptionError,"UnrecognizedOption",option)
      }
      case 'v':
      {
        if (LocaleCompare("verbose",option+1) == 0)
          break;
        if (LocaleCompare("virtual-pixel",option+1) == 0)
          {
            long
              method;

            if (*option == '+')
              break;
            i++;
            if (i == (long) (argc-1))
              ThrowIdentifyException(OptionError,"MissingArgument",option);
            method=ParseMagickOption(MagickVirtualPixelOptions,MagickFalse,
              argv[i]);
            if (method < 0)
              ThrowIdentifyException(OptionError,
                "UnrecognizedVirtualPixelMethod",argv[i]);
            break;
          }
        ThrowIdentifyException(OptionError,"UnrecognizedOption",option)
      }
      case '?':
        break;
      default:
        ThrowIdentifyException(OptionError,"UnrecognizedOption",option)
    }
    fire=ParseMagickOption(MagickMogrifyOptions,MagickFalse,option+1) < 0 ? 
      MagickFalse : MagickTrue;
    if (fire != MagickFalse)
      FireImageStack(MagickFalse,MagickTrue,MagickTrue);
  }
  if (k != 0)
    ThrowIdentifyException(OptionError,"UnbalancedParenthesis",argv[i]);
  if (i != argc)
    ThrowIdentifyException(OptionError,"MissingAnImageFilename",argv[i]);
  DestroyIdentify();
  return(status != 0 ? MagickTrue : MagickFalse);
}
