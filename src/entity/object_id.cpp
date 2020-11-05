#include "object_id.h"


const int ObjectID::AutoSectionBegin = 3_0000_0000;
const int ObjectID::AutoSectionBegin = 3_9999_9999;
// 用户号段
const int ObjectID::UserSectionBegin = 100_0000;
const int ObjectID::UserSectionBegin = 199_9999;
// 系统号段
const int ObjectID::SystemSectionBegin = 1000;
const int ObjectID::SystemSectionBegin = 9999;


ObjectID::s_NextAutoID  = ObjectID::AutoSectionBegin;
