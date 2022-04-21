/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%                         W   W   AAA   N   N  DDDD                           %
%                         W   W  A   A  NN  N  D   D                          %
%                         W W W  AAAAA  N N N  D   D                          %
%                         WW WW  A   A  N  NN  D   D                          %
%                         W   W  A   A  N   N  DDDD                           %
%                                                                             %
%                                                                             %
%                           Wand Support Methods                              %
%                                                                             %
%                              Software Design                                %
%                                John Cristy                                  %
%                                 May  2004                                   %
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
%
*/

/*
  Include declarations.
*/
#include "wand/studio.h"
#include "wand/MagickWand.h"
#include "wand/magick-wand-private.h"
#include "wand/wand.h"

static SplayTreeInfo
  *wand_ids = (SplayTreeInfo *) NULL;

static MagickBooleanType
  instantiate_wand = MagickFalse;

static SemaphoreInfo
  *wand_semaphore = (SemaphoreInfo *) NULL;

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   A c q u i r e W a n d I d                                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  AcquireWandId() returns a unique wand id.
%
%  The format of the AcquireWandId() method is:
%
%      unsigned long AcquireWandId()
%
*/
WandExport unsigned long AcquireWandId(void)
{
  static unsigned long
    id = 0;

  if ((wand_ids == (SplayTreeInfo *) NULL) &&
      (instantiate_wand == MagickFalse))
    {
      AcquireSemaphoreInfo(&wand_semaphore);
      if ((wand_ids == (SplayTreeInfo *) NULL) &&
          (instantiate_wand == MagickFalse))
        {
          wand_ids=NewSplayTree((int (*)(const void *,const void *)) NULL,
            (void *(*)(void *)) NULL,(void *(*)(void *)) NULL);
          instantiate_wand=MagickTrue;
        }
      RelinquishSemaphoreInfo(wand_semaphore);
    }
  AcquireSemaphoreInfo(&wand_semaphore);
  id++;
  (void) AddValueToSplayTree(wand_ids,(const void *) id,(const void *) id);
  RelinquishSemaphoreInfo(wand_semaphore);
  return(id);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%    R e l i n q u i s h W a n d I d                                          %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  RelinquishWandId() relinquishes a unique wand id.
%
%  The format of the RelinquishWandId() method is:
%
%      void RelinquishWandId(const unsigned long *id)
%
%  A description of each parameter follows:
%
%    o id: a unique wand id.
%
*/
WandExport void RelinquishWandId(const unsigned long id)
{
  (void) LogMagickEvent(WandEvent,GetMagickModule(),"...");
  if (wand_ids == (SplayTreeInfo *) NULL)
    return;
  AcquireSemaphoreInfo(&wand_semaphore);
  (void) DeleteNodeByValueFromSplayTree(wand_ids,(const void *) id);
  if ((instantiate_wand == MagickFalse) ||
      (GetNumberOfNodesInSplayTree(wand_ids) != 0))
    {
      RelinquishSemaphoreInfo(wand_semaphore);
      return;
    }
  wand_ids=DestroySplayTree(wand_ids);
  instantiate_wand=MagickFalse;
  RelinquishSemaphoreInfo(wand_semaphore);
  DestroySemaphoreInfo(&wand_semaphore);
}
