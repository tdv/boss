#ifndef __BOSS_CORE_ERROR_CODES_H__
#define __BOSS_CORE_ERROR_CODES_H__

namespace Boss
{

  namespace Status
  {

    enum
    {
      Ok,
      False,
      Fail,
      NotFound,
      AlreadyExists,
      InvalidArgument,
      NotImplemented
    };

  }

}

#endif  // !__BOSS_CORE_ERROR_CODES_H__
