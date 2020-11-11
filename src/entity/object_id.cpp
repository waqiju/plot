#include "object_id.h"


const int ObjectID::AutoSectionBegin = 300000000;
const int ObjectID::AutoSectionEnd = 399999999;
// 用户号段
const int ObjectID::UserSectionBegin = 1000000;
const int ObjectID::UserSectionEnd = 1999999;
// 系统号段
const int ObjectID::SystemSectionBegin = 1000;
const int ObjectID::SystemSectionEnd = 9999;
const int ObjectID::WorldRoot = 1000;
const int ObjectID::PlotRoot = 1001;


int ObjectID::s_NextAutoID  = ObjectID::AutoSectionBegin;
