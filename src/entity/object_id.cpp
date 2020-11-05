#include "object_id.h"


const int ObjectID::AutoSectionBegin = 3_0000_0000;
const int ObjectID::AutoSectionEnd = 3_9999_9999;
// 用户号段
const int ObjectID::UserSectionBegin = 100_0000;
const int ObjectID::UserSectionEnd = 199_9999;
// 系统号段
const int ObjectID::SystemSectionBegin = 1000;
const int ObjectID::SystemSectionEnd = 9999;
const int ObjectID::WorldRoot = 1000;
const int ObjectID::PlotRoot = 1001;


ObjectID::s_NextAutoID  = ObjectID::AutoSectionBegin;
