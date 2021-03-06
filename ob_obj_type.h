/**
 * (C) 2010-2012 Alibaba Group Holding Limited.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * Version: $Id$
 *
 * ob_obj_type.h
 *
 * Authors:
 *   zhidong<xielun.szd@alipay.com>
 *
 */

#ifndef OCEANBASE_COMMON_OB_OBJECT_TYPE_H_
#define OCEANBASE_COMMON_OB_OBJECT_TYPE_H_

#include "stdint.h"

namespace jdbd
{
    namespace common
    {
        // Obj类型只能增加，不能删除，顺序也不能变，见ob_obj_cast.h

        enum ObObjType
        {
            ObMinType = -1,

            ObNullType, // 空类型
            ObIntType,
            ObFloatType, // @deprecated

            ObDoubleType, // @deprecated
            ObDateTimeType, // @deprecated
            ObPreciseDateTimeType, // =5

            ObVarcharType,
            ObSeqType,
            ObCreateTimeType,

            ObModifyTimeType,
            ObExtendType,
            ObBoolType,

            ObDecimalType, // aka numeric
            ObMaxType,
        };
        // print obj type string
        const char* ob_obj_type_str(ObObjType type);
        // get obj type size for fixed length type
        int64_t ob_obj_type_size(ObObjType type);
        ObObjType trans_int_type2obj_type(int type);
    }
}

#endif //OCEANBASE_COMMON_OB_OBJECT_TYPE_H_
